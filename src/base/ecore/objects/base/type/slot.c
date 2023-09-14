#include "slot.h"

#include <ecore/base/extra.h>

#include <ecore/objects/base/typecore.h>
#include <ecore/vm/builtins/builtins.h>
#include <ecore/vm/memory/arena.h>

#include <ecore/objects/base/type_transfer.h>
#include <ecore/objects/base/object.h>
#include <ecore/objects/base/molecule/molecule.h>
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
    // slot->referenced_types will be initialized by other code
    slot->interface = Eco_Interface_GetDefaultInterface();
    Eco_BasicSlotInfo_Create(&slot->info);
}

bool Eco_TypeSlot_GetValue(struct Eco_TypeSlot* slot, struct Eco_Molecule* molecule, Eco_Any* location)
{
    switch (slot->type)
    {
        case Eco_TypeSlotType_INLINED:
            *location = *((Eco_Any*) Eco_Molecule_At(molecule, slot->body.inlined.offset));
            return true;
        default:
            return false;
    }
}

bool Eco_TypeSlot_SetValue(struct Eco_Type* type, unsigned int index, struct Eco_Molecule* molecule, Eco_Any value)
{
    struct Eco_TypeSlot*  slot;

    slot = &(type->slots[index]);
    switch (slot->type)
    {
        case Eco_TypeSlotType_INLINED:
            *((Eco_Any*) Eco_Molecule_At(molecule, slot->body.inlined.offset)) = value;
            if (slot->info.flags.is_with) {
                Eco_TypeTransfer(molecule, index, Eco_Any_GetType(value));
            }
            return true;
        default:
            return false;
    }
}

bool Eco_TypeSlot_Invoke(struct Eco_Message*   message,
                         struct Eco_Molecule*  molecule,
                         struct Eco_Type*      type,
                         unsigned int          index,
                         Eco_Any               self)
{
    struct Eco_TypeSlot*  slot;

    slot = &type->slots[index];
    switch (message->type)
    {
        case Eco_Message_Type_SEND:
            switch (slot->type)
            {
                case Eco_TypeSlotType_INLINED:
                    Eco_Fiber_Drop(message->fiber);  /* Drop self */
                    Eco_Fiber_SetAccu(message->fiber, *((Eco_Any*) Eco_Molecule_At(molecule, slot->body.inlined.offset)));
                    return true;
                case Eco_TypeSlotType_CODE:
                    *Eco_Fiber_Nth(message->fiber, message->body.send.arg_count) = self;   // Assign the new self
                    return Eco_Fiber_Enter(message->fiber, Eco_Any_FromPointer(molecule), NULL, slot->body.code.code, message->body.send.arg_count);
            }
            return false;
        case Eco_Message_Type_ASSIGN:
            Eco_Fiber_SetAccu(message->fiber, Eco_Fiber_Pop(message->fiber));
            return Eco_TypeSlot_SetValue(type, index, molecule, message->body.assign.value);
        default:
            return false;
    }
}
