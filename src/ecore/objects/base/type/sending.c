#include <ecore/objects/molecule/molecule.h>
#include <ecore/objects/vm/interface/interface.h>

#include "sending.h"


bool Eco_Type_SendMessageToMolecule(struct Eco_Message*  message,
                                    struct Eco_SendLink* link,
                                    Eco_Any              self,
                                    struct Eco_Type*     type,
                                    struct Eco_Molecule* molecule)
{
    struct Eco_TypeSlot* slot;
    Eco_Any              value;
    unsigned int         i;
    bool                 result;

    for (i = 0; i < type->slot_count; i++)
    {
        slot = &type->slots[i];
        if (slot->info.key == message->key) {
            if (slot->info.flags.is_private && !message->private_send)
                continue;
            Eco_TypeSlot_Invoke(message, molecule, slot, self);
            return true;
        }
    }

    for (i = 0; i < type->slot_count; i++)
    {
        slot = &(type->slots[i]);

        if (slot->info.flags.is_private && !message->private_send)
            continue;

        switch (slot->type)
        {
            case Eco_TypeSlotType_INLINED:
                if (slot->info.flags.is_inherited && Eco_Interface_ImplementsMessage(slot->interface, message->key)) {
                    if (Eco_TypeSlot_GetValue(slot, molecule, &value)) {
                        if (slot->info.flags.is_delegate) {
                            result = Eco_Send(message, link, value, value, message->private_send);
                        } else {
                            result = Eco_Send(message, link, value, self, message->private_send);
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