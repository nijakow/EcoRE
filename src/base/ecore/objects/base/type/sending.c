#include <ecore/objects/base/molecule/molecule.h>
#include <ecore/objects/vm/interface/interface.h>

#include "sending.h"


bool Eco_Type_SendMessageToMolecule(struct Eco_Message*  message,
                                    Eco_Any              self,
                                    struct Eco_Type*     type,
                                    struct Eco_Molecule* molecule,
                                    bool                 is_delegated)
{
    struct Eco_TypeSlot* slot;
    Eco_Any              value;
    unsigned int         i;
    bool                 result;

    for (i = type->slot_count; i-- > 0;)
    {
        slot = &type->slots[i];
        if (slot->info.key == message->key) {
            if ((slot->info.flags.is_protected && !message->private_send) || (slot->info.flags.is_static && is_delegated))
                continue;
            if (slot->info.perspective != NULL)
                continue;
            Eco_TypeSlot_Invoke(message, molecule, type, i, self);
            return true;
        }
    }

    for (i = type->slot_count; i-- > 0;)
    {
        slot = &(type->slots[i]);

        if ((slot->info.flags.is_protected && !message->private_send) || (slot->info.flags.is_static && is_delegated))
            continue;

        switch (slot->type)
        {
            case Eco_TypeSlotType_INLINED:
                if (slot->info.flags.is_with && Eco_Interface_ImplementsMessage(slot->interface, message->key)) {
                    if (Eco_TypeSlot_GetValue(slot, molecule, &value)) {
                        /*
                         * Check for circular inheritance (only valid if the slot points to SELF)
                         */
                        if (Eco_Any_IsPointer(value) && Eco_Any_AsPointer(value) == molecule)
                            continue;
                        if (slot->info.flags.is_inherited) {
                            result = Eco_Send(message, value, self, message->private_send, true);
                        } else {
                            result = Eco_Send(message, value, value, message->private_send, true);
                        }
                        if (result) return true;
                    }
                }
                break;
            default:
                break;
        }
    }

    return false;
}
