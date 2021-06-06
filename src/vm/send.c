#include "send.h"

#include "../objects/object.h"
#include "../objects/type.h"




bool Eco_Send(struct Eco_Message* message, Eco_Any* target)
{
    struct Eco_Object*  object;

    switch (target->type)
    {
    case Eco_Value_Type_POINTER:
        object = Eco_Any_AsPointer(target);
        return object->type->shared->send(message, object);
    default:
        return false;   /* TODO */
    }
}