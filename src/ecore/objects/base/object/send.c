#include "object.h"
#include "send.h"

#include <ecore/objects/base/type.h>
#include <ecore/vm/core/send.h>
#include <ecore/objects/vm/interface/interface.h>

bool Eco_Object_Send(struct Eco_Message* message,
                     struct Eco_SendLink* link,
                     struct Eco_Object* target,
                     Eco_Any* self)
{
    unsigned int          i;
    Eco_Any               value;
    struct Eco_Type*      type;
    struct Eco_TypeSlot*  slot;
    bool                  result;


    if (target == NULL) return false;

    type = target->type;

    for (i = 0; i < type->slot_count; i++)
    {
        slot = &type->slots[i];
        if (slot->key == message->key) {
            Eco_TypeSlot_Invoke(message, target, slot, self);
            return true;
        }
    }

    for (i = 0; i < type->slot_count; i++)
    {
        slot = &(type->slots[i]);

        switch (slot->type)
        {
            case Eco_TypeSlotType_INLINED:
                if (slot->flags.is_inherited && Eco_Interface_ImplementsMessage(slot->interface, message->key)) {
                    if (Eco_TypeSlot_GetValue(slot, target, &value)) {
                        if (slot->flags.is_delegate) {
                            result = Eco_Send(message, link, &value, &value);
                        } else {
                            result = Eco_Send(message, link, &value, self);
                        }
                        if (result) return true;
                    }
                }
                break;
            case Eco_TypeSlotType_SHARED:
                if (slot->flags.is_inherited && Eco_Interface_ImplementsMessage(slot->interface, message->key)) {
                    //if (slot->body.shared.is_delegate) {  // TODO
                        result = Eco_Send(message, link, &slot->body.shared.value, &slot->body.shared.value);
                    /*} else {
                        result = Eco_Send(message, link, &slot->body.shared.value, self);
                    }*/
                    if (result) return true;
                }
                break;
            default:
                break;
        }
    }

    if (target->type->proxy != NULL) {
        return Eco_Send_ToObject(message, link, target->type->proxy, self);
    } else {
        return false;
    }
}
