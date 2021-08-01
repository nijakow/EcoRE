#include "send.h"

#include "../../objects/base/object.h"
#include "../../objects/base/type.h"


bool Eco_Send(struct Eco_Message* message, Eco_Any* target)
{
    struct Eco_Object*  object;

    if (Eco_Any_IsPointer(target)) {
        object = Eco_Any_AsPointer(target);
        return object->type->typecore->send(message, object);
    } else {
        return false;   // TODO
    }
}