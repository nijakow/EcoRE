#include "type.h"

#include <ecore/base/extra.h>

#include <ecore/objects/base/typecore.h>
#include <ecore/vm/builtins/builtins.h>
#include <ecore/vm/memory/arena.h>

#include <ecore/objects/base/object.h>
#include <ecore/objects/molecule/molecule.h>
#include <ecore/objects/misc/key/key.h>
#include <ecore/objects/vm/code/code.h>
#include <ecore/objects/vm/code/closure.h>
#include <ecore/objects/vm/interface/interface.h>
#include <ecore/vm/core/clone.h>
#include <ecore/vm/core/send.h>
#include <ecore/vm/fiber/fiber.h>
#include <ecore/vm/fiber/stackops.h>
#include <ecore/vm/core/interpreter.h>
#include <ecore/vm/memory/memory.h>
#include <ecore/vm/memory/gc/gc.h>
#include <ecore/io/logging/log.h>


struct Eco_Arena  Eco_TYPES;

struct Eco_TypeCore Eco_Type_TYPECORE;
struct Eco_Type*    Eco_Type_TYPE;


struct Eco_Type* Eco_Type_New(unsigned int slot_count)
{
    struct Eco_Type*  type;

    type = Eco_Object_NewInArena(Eco_Type_TYPE,
                                 sizeof(struct Eco_Type) + sizeof(struct Eco_TypeSlot) * slot_count,
                                 &Eco_TYPES);

    type->typecore              = NULL;
    type->proxy                 = NULL;
    type->interface             = NULL;
    type->public_interface      = NULL;
    type->slot_count            = slot_count;
    type->instance_payload_size = 0;

    return type;
}

void Eco_Type_Del(struct Eco_Type* type)
{
    Eco_Object_Del(&type->_);
}

struct Eco_Type* Eco_Type_NewPrefab(struct Eco_TypeCore* typecore)
{
    struct Eco_Type* type;

    type = Eco_Type_New(0);

    type->typecore              = typecore;
    type->instance_payload_size = 0;
    type->_.bits.sticky         = true;

    return type;
}

void Eco_Type_Mark(struct Eco_GC_State* state, struct Eco_Type* type)
{
    unsigned int  i;

    for (i = 0; i < type->slot_count; i++)
    {
        Eco_GC_State_MarkObject(state, type->slots[i].info.key);
        Eco_GC_State_MarkObject(state, (struct Eco_Object*) type->slots[i].interface);
        switch (type->slots[i].type)
        {
            case Eco_TypeSlotType_INLINED:
                break;
            case Eco_TypeSlotType_SHARED:
                Eco_GC_State_MarkAny(state, &type->slots[i].body.shared.value);
                break;
            case Eco_TypeSlotType_CODE:
                Eco_GC_State_MarkObject(state, type->slots[i].body.code.code);
                break;
        }
    }
    if (type->proxy != NULL)
        Eco_GC_State_MarkObject(state, type->proxy);
    if (type->interface != NULL)
        Eco_GC_State_MarkObject(state, type->interface);
    if (type->public_interface != NULL)
        Eco_GC_State_MarkObject(state, type->public_interface);
    Eco_Object_Mark(state, &type->_);
}


void Eco_Type_MarkMolecule(struct Eco_GC_State* state,
                           struct Eco_Type* type,
                           struct Eco_Molecule* molecule)
{
    unsigned int  i;

    for (i = 0; i < type->slot_count; i++)
    {
        switch (type->slots[i].type)
        {
            case Eco_TypeSlotType_INLINED:
                Eco_GC_State_MarkAny(state, Eco_Molecule_At(molecule, type->slots[i].body.inlined.offset));
                break;
            default:
                break;
        }
    }
}


void Eco_Type_Subclone(struct Eco_CloneState* state,
                       struct Eco_Type* type,
                       struct Eco_Molecule* original,
                       struct Eco_Molecule* clone)
{
    unsigned int  i;

    for (i = 0; i < type->slot_count; i++)
    {
        switch (type->slots[i].type)
        {
            case Eco_TypeSlotType_INLINED:
                Eco_CloneState_CloneAny(state,
                                        Eco_Molecule_At(clone, type->slots[i].body.inlined.offset),
                                        Eco_Molecule_At(original, type->slots[i].body.inlined.offset),
                                        type->slots[i].info.flags.is_part);
                break;
            default:
                break;
        }
    }
}


struct Eco_Interface* Eco_Any_GetInterface(Eco_Any value, bool private_also)
{
    struct Eco_Type* type;

    type = Eco_Any_GetType(value);
    return Eco_Type_GetInterface(type, Eco_Any_IsPointer(value) ? Eco_Any_AsPointer(value) : NULL, private_also);
}

struct Eco_Interface* Eco_Type_GetInterface(struct Eco_Type*   type,
                                            struct Eco_Object* object,
                                            bool               private_also)
{
    struct Eco_Interface*  interface;
    unsigned int           index;
    unsigned int           parent_index;
    unsigned int           slot_index;
    unsigned int           inherited_slot_count;
    unsigned int           slot_count;
    Eco_Any                value;

    /*
     * If the type has a proxy, use the proxy's interface instead.
     *
     * TODO, FIXME, XXX:
     * This is a very ugly fix. It ignores the slots already located
     * within the type, but it gives us an opportunity to get the
     * interface of e.g. strings and integers without too much hassle.
     *                                                - nijakow
     */
    if (type->proxy != NULL) {
        return Eco_Type_GetInterface(type->proxy->type, type->proxy, private_also);
    }

    /*
     * Check if we already have the interface pointer set.
     * If so, return its stored value.
     */
    if (private_also) {
        if (type->interface != NULL)
            return type->interface;
    } else {
        if (type->public_interface != NULL)
            return type->public_interface;
    }
    
    /*
     * The interface has not been computed yet, so we prepare ourselves
     * for creating a new interface instance. For that, we need to know
     * how many slots and parents it will have.
     * 
     * We therefore run a basic loop, incrementing the corresponding
     * counters.
     */
    inherited_slot_count = 0;
    slot_count           = 0;
    for (index = 0; index < type->slot_count; index++)
    {
        if (private_also || !type->slots[index].info.flags.is_private) {
            if (type->slots[index].info.flags.is_inherited)
                inherited_slot_count++;
            slot_count++;
        }
    }

    /*
     * Everything's ready, we can now allocate.
     * 
     * TODO, FIXME, XXX: There is no check if the interface is NULL!
     */
    interface = Eco_Interface_New(inherited_slot_count, slot_count);

    /*
     * Update the interface pointers to point to our newly
     * created interface. We do this before setting up the
     * contents of our interface so that we can avoid
     * infinite recursion pitfalls.
     */
    if (private_also) {
        type->interface = interface;
    } else {
        type->public_interface = interface;
    }

    /*
     * Now we do the actual copying of the slot names and types
     * and all the other important stuff into the interface.
     */
    parent_index = 0;
    slot_index   = 0;
    for (index = 0; index < type->slot_count; index++)
    {
        if (!private_also && type->slots[index].info.flags.is_private)
            continue;
        if (type->slots[index].info.flags.is_inherited) {
            if (Eco_TypeSlot_GetValue(&type->slots[index], (struct Eco_Molecule*) object, &value))
                interface->parents[parent_index] = Eco_Any_GetInterface(value, private_also);
            else
                interface->parents[parent_index] = NULL;  // TODO: Error
            parent_index++;
        }
        interface->entries[slot_index].return_type = type->slots[index].interface;
        interface->entries[slot_index].key         = type->slots[index].info.key;
        interface->entries[slot_index].arg_count   = 0;      // TODO: If it's a method, check for args
        interface->entries[slot_index].has_varargs = false;  // TODO: If it's a method, check for varargs
        slot_index++;
    }

    /*
     * Done, return the interface.
     */
    return interface;
}


void Eco_Types_Init()
{
    Eco_Arena_Create(&Eco_TYPES);
    Eco_TypeCore_Create(&Eco_Type_TYPECORE, "Eco_Type");

    Eco_Type_TYPECORE.send  = (Eco_TypeCore_SendFunc) NULL;  // TODO, FIXME, XXX
    Eco_Type_TYPECORE.mark  = (Eco_TypeCore_MarkFunc) Eco_Type_Mark;
    Eco_Type_TYPECORE.del   = (Eco_TypeCore_DelFunc) Eco_Type_Del;

    Eco_Type_TYPE           = Eco_Type_NewPrefab(&Eco_Type_TYPECORE);

    Eco_Type_TYPE->_.type   = Eco_Type_TYPE;
}

void Eco_Types_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_Type_TYPECORE);
    Eco_Arena_Destroy(&Eco_TYPES);
}
