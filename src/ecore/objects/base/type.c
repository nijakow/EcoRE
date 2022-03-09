#include "type.h"

#include <ecore/objects/base/typecore.h>
#include <ecore/vm/builtins/builtins.h>
#include <ecore/vm/memory/arena.h>

#include <ecore/objects/base/object.h>
#include <ecore/objects/molecule/molecule.h>
#include <ecore/objects/misc/key/key.h>
#include <ecore/objects/vm/code/code.h>
#include <ecore/objects/vm/code/closure.h>
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


bool Eco_TypeSlot_GetValue(struct Eco_TypeSlot* slot, struct Eco_Object* object, Eco_Any* location)
{
    switch (slot->type)
    {
        case Eco_TypeSlotType_INLINED:
            *location = *((Eco_Any*) Eco_Molecule_At((struct Eco_Molecule*) object, slot->body.inlined.offset));
            return true;
        default:
            return false;
    }
}

bool Eco_TypeSlot_SetValue(struct Eco_TypeSlot* slot, struct Eco_Object* object, Eco_Any* value)
{
    switch (slot->type)
    {
        case Eco_TypeSlotType_INLINED:
            *((Eco_Any*) Eco_Molecule_At((struct Eco_Molecule*) object, slot->body.inlined.offset)) = *value;
            return true;
        default:
            return false;
    }
}

bool Eco_TypeSlot_Invoke(struct Eco_Message*   message,
                         struct Eco_Object*    object,
                         struct Eco_TypeSlot*  slot,
                         Eco_Any*              self)
{
    switch (message->type)
    {
        case Eco_Message_Type_SEND:
            switch (slot->type)
            {
                case Eco_TypeSlotType_INLINED:
                    Eco_Fiber_Drop(message->fiber);  /* Drop self */
                    Eco_Fiber_Push(message->fiber, ((Eco_Any*) Eco_Molecule_At((struct Eco_Molecule*) object, slot->body.inlined.offset)));
                    return true;
                case Eco_TypeSlotType_SHARED:
                    Eco_Fiber_Drop(message->fiber);
                    Eco_Fiber_Push(message->fiber, &slot->body.shared.value);
                    return true;
                case Eco_TypeSlotType_CODE:
                    Eco_Any_AssignAny(Eco_Fiber_Nth(message->fiber, message->body.send.arg_count), self);   // Assign the new self
                    return Eco_Fiber_Enter(message->fiber, NULL, slot->body.code.code, message->body.send.arg_count);
            }
            return false;
        case Eco_Message_Type_ASSIGN:
            switch (slot->type)
            {
                case Eco_TypeSlotType_INLINED:
                    Eco_Any_AssignAny((Eco_Any*) Eco_Molecule_At((struct Eco_Molecule*) object, slot->body.inlined.offset), &message->body.assign.value);
                    return true;
                default:
                    return false;
            }
        default:
            return false;
    }
}


static struct Eco_Type* Eco_Type_New(unsigned int slot_count)
{
    struct Eco_Type*  type;

    type = Eco_Object_NewInArena(Eco_Type_TYPE,
                                 sizeof(struct Eco_Type) + sizeof(struct Eco_TypeSlot) * slot_count,
                                 &Eco_TYPES);

    type->typecore              = NULL;
    type->proxy                 = NULL;
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

static bool Eco_Type_CopyWithNewSlot(struct Eco_Type*      self,
                                     int                   pos,
                                     struct Eco_Type**     type_loc,
                                     struct Eco_TypeSlot** slot_loc)
{
    unsigned int      i;
    unsigned int      adjusted_pos;
    struct Eco_Type*  the_copy;

    const unsigned int  new_slot_count = self->slot_count + 1;

    if (pos >= 0) adjusted_pos =  pos;
    else          adjusted_pos = self->slot_count + pos + 1;

    if (adjusted_pos >= new_slot_count) adjusted_pos = new_slot_count - 1;


    the_copy                        = Eco_Type_New(new_slot_count);
    *type_loc                       = the_copy;
    the_copy->typecore              = self->typecore;
    the_copy->instance_payload_size = self->instance_payload_size;

    for (i = 0; i < new_slot_count; i++) {
        if (i < adjusted_pos) the_copy->slots[i] = self->slots[i];
        else if (i == adjusted_pos) *slot_loc = &the_copy->slots[i];
        else the_copy->slots[i] = self->slots[i - 1];
    }

    *type_loc = the_copy;

    return true;
}

bool Eco_Type_CopyWithNewInlinedSlot(struct Eco_Type*           self,
                                     int                        pos,
                                     struct Eco_Object_SlotInfo info,
                                     struct Eco_Type**          type_loc,
                                     struct Eco_TypeSlot**      slot_loc)
{
    struct Eco_Type*      the_copy;
    struct Eco_TypeSlot*  the_slot;

    const unsigned int     slot_value_size  = sizeof(Eco_Any);

    the_slot                                = NULL;

    if (Eco_Type_CopyWithNewSlot(self, pos, &the_copy, &the_slot)) {
        the_copy->instance_payload_size += slot_value_size;

        the_slot->type                    = Eco_TypeSlotType_INLINED;
        the_slot->key                     = info.key;
        the_slot->flags.is_inherited      = info.is_inherited;
        the_slot->flags.is_delegate       = info.is_delegate;
        the_slot->flags.is_part           = info.is_part;
        the_slot->body.inlined.value_size = slot_value_size;
        the_slot->body.inlined.offset     = the_copy->instance_payload_size - slot_value_size;

        *type_loc                         = the_copy;
        *slot_loc                         = the_slot;
        
        return true;
    } else {
        return false;
    }
}

bool Eco_Type_CopyWithNewCodeSlot(struct Eco_Type* self,
                                  int pos,
                                  struct Eco_Object_SlotInfo info,
                                  struct Eco_Code* code,
                                  struct Eco_Type** type_loc)
{
    struct Eco_Type*       the_copy;
    struct Eco_TypeSlot*  the_slot;

    const unsigned int     slot_value_size  = sizeof(Eco_Any);
    the_slot                                = NULL;

    if (Eco_Type_CopyWithNewSlot(self, pos, &the_copy, &the_slot)) {
        the_copy->instance_payload_size += slot_value_size;

        the_slot->type           = Eco_TypeSlotType_CODE;
        the_slot->key            = info.key;
        the_slot->body.code.code = code;

        *type_loc                = the_copy;

        return true;
    } else {
        return false;
    }
}

bool Eco_Type_CopyWithRemovedSlot(struct Eco_Type* self,
                                  unsigned int pos,
                                  struct Eco_Type** type_loc)
{
    unsigned int      i;
    unsigned int      adjusted_pos;
    struct Eco_Type*  the_copy;

    const unsigned int  new_slot_count = self->slot_count - 1;

    if (pos >= self->slot_count) adjusted_pos = self->slot_count - 1;
    else                         adjusted_pos = pos;

    the_copy                        = Eco_Type_New(new_slot_count);
    *type_loc                       = the_copy;
    the_copy->typecore              = self->typecore;
    the_copy->instance_payload_size = self->instance_payload_size;

    for (i = 0; i < self->slot_count; i++) {
        if (i < adjusted_pos) the_copy->slots[i] = self->slots[i];
        else if (i == adjusted_pos) continue;
        else the_copy->slots[i - 1] = self->slots[i];
    }

    *type_loc = the_copy;

    return true;
}


void Eco_Type_Mark(struct Eco_GC_State* state, struct Eco_Type* type)
{
    unsigned int  i;

    for (i = 0; i < type->slot_count; i++)
    {
        Eco_GC_State_MarkObject(state, type->slots[i].key);
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
                                        type->slots[i].flags.is_part);
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
