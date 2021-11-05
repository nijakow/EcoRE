#include "send.h"

#include <ecore/objects/base/object.h>
#include <ecore/objects/base/type.h>
#include <ecore/objects/pseudo/integer.h>


bool Eco_Send_ToObject(struct Eco_Message* message,
                       struct Eco_SendLink* link,
                       struct Eco_Object* object)
{
    struct Eco_SendLink  next_link;

    /*
     * In order to avoid bottomless recursion, we cons up
     * a linked list of all visited objects on the stack.
     */
    next_link.previous = link;
    next_link.object   = object;

    /*
     * Every time we encounter an object that we have already
     * visited, we abort the recursive search. Let's loop over
     * the linked list and scan for references to `object`.
     */
    while (link != NULL)
    {
        if (link->object == object) {
            return false;
        }
        link = link->previous;
    }

    /*
     * Dispatch to the local send handler.
     */
    return object->type->typecore->send(message, &next_link, object);
}

bool Eco_Send(struct Eco_Message* message, struct Eco_SendLink* link, Eco_Any* target)
{
    if (Eco_Any_IsPointer(target)) {
        return Eco_Send_ToObject(message, link, Eco_Any_AsPointer(target));
    } else if (Eco_Any_IsInteger(target)) {
        if (Eco_Integer_TYPE->proxy != NULL)
            return Eco_Send_ToObject(message, link, Eco_Integer_TYPE->proxy);
        else
            return false;
    } else {
        return false;   // TODO
    }
}