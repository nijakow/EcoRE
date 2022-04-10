#include "send.h"

#include <ecore/objects/base/object.h>
#include <ecore/objects/base/type.h>
#include <ecore/objects/pseudo/integer.h>
#include <ecore/objects/pseudo/character.h>


bool Eco_Send_ToObject(struct Eco_Message*  message,
                       struct Eco_Object*   object,
                       Eco_Any              self)
{
    /*
     * Dispatch to the local send handler.
     */
    return object->type->typecore->send(message, object, self);
}


static struct Eco_Type** Eco_Send_JUMPS[] = {
    [Eco_Value_Type_POINTER]   =  NULL,
    [Eco_Value_Type_INTEGER]   = &Eco_Integer_TYPE,
    [Eco_Value_Type_CHARACTER] = &Eco_Character_TYPE,
    [Eco_Value_Type_FLOATING]  =  NULL
};

bool Eco_Send(struct Eco_Message*  message,
              Eco_Any              target,
              Eco_Any              self,
              bool                 private_send)
{
    struct Eco_Object*  object;

    message->private_send = private_send;
    if (Eco_Any_IsPointer(target)) {
        object = Eco_Any_AsPointer(target);
    } else {
        object = (*Eco_Send_JUMPS[Eco_Any_GetValueType(target)])->proxy;
    }
    return Eco_Send_ToObject(message, object, self);
}
