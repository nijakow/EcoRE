#include "type.h"

#include "object.h"
#include "../vm/send.h"


bool Eco_Type_Slot_GetValue(struct Eco_Type_Slot* slot, struct Eco_Object* object, Eco_Any* location)
{
    switch (slot->type)
    {
        case Eco_Type_Slot_Type_INLINED:
            *location = *((Eco_Any*) Eco_Object_At(object, slot->body.inlined.offset));
            return true;
        default:
            return false;
    }
}

bool Eco_Type_Slot_Invoke(struct Eco_Message* message, struct Eco_Object* object, struct Eco_Type_Slot* slot)
{
    switch (message->type)
    {
        case Eco_Message_Type_SEND:
            switch (slot->type)
            {
                case Eco_Type_Slot_Type_INLINED:
                    message->body.send.arg_location[0] = *((Eco_Any*) Eco_Object_At(object, slot->body.inlined.offset));
                    return true;
                case Eco_Type_Slot_Type_CODE:
                    /* TODO */
                    return false;
            }
            return false;
        case Eco_Message_Type_ASSIGN:
            switch (slot->type)
            {
                case Eco_Type_Slot_Type_INLINED:
                    *((Eco_Any*) Eco_Object_At(object, slot->body.inlined.offset)) = *(message->body.assign.value);
                    return true;
            }
            return false;
        default:
            return false;
    }
}
