#include "send.h"

#include "../objects/object.h"
#include "../objects/type.h"


bool Eco_Send_ToObject(struct Eco_Message* message, struct Eco_Object* target)
{
    unsigned int           i;
    struct Eco_Type*       type;
    struct Eco_Type_Slot*  slot;

    type = target->type;

    for (i = 0; i < type->slot_count; i++)
    {
        if (type->slots[i].key == message->key) {
            /* TODO */
        }
    }

    for (i = 0; i < type->slot_count; i++)
    {
        slot = &(type->slots[i]);

        if (slot->type == Eco_Type_Slot_Type_INLINED) {
            if (slot->body.inlined.is_inherited) {
                /* TODO */
            }
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