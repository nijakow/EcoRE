#include "slot.h"

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


void Eco_TypeSlot_Initialize(struct Eco_TypeSlot* slot)
{
    // slot->type is currently left uninitialized
    slot->interface = Eco_Interface_GetDefaultInterface();
    Eco_BasicSlotInfo_Create(&slot->info);
}

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
