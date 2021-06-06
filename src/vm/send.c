#include "send.h"

#include "../objects/object.h"
#include "../objects/type.h"


bool Eco_Send_ToObject(struct Eco_Message* message, struct Eco_Object* target)
{
    unsigned int           i;
    Eco_Any                value;
    struct Eco_Type*       type;
    struct Eco_Type_Slot*  slot;

    type = target->type;

    for (i = 0; i < type->slot_count; i++)
    {
        if (type->slots[i].key == message->key) {
            Eco_Type_Slot_Invoke(message, target, &(type->slots[i]));
            return true;
        }
    }

    for (i = 0; i < type->slot_count; i++)
    {
        slot = &(type->slots[i]);

        switch (slot->type)
        {
            case Eco_Type_Slot_Type_INLINED:
                if (slot->body.inlined.is_inherited) {
                    if (Eco_Type_Slot_GetValue(slot, target, &value)) {
                        if (Eco_Send(message, &value)) {
                            return true;
                        }
                    }
                }
                break;
            default:
                break;
        }
    }

    return false;
}


bool Eco_Send(struct Eco_Message* message, Eco_Any* target)
{
    switch (target->type)
    {
    case Eco_Value_Type_POINTER:
        return Eco_Send_ToObject(message, Eco_Any_AsPointer(target));
    default:
        return false;   /* TODO */
    }
}