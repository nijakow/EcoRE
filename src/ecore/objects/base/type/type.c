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
