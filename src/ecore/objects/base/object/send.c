#include "object.h"
#include "send.h"

#include <ecore/objects/base/type.h>
#include <ecore/objects/base/type/sending.h>
#include <ecore/vm/core/send.h>
#include <ecore/objects/vm/interface/interface.h>

bool Eco_Object_Send(struct Eco_Message*  message,
                     struct Eco_SendLink* link,
                     struct Eco_Object*   target,
                     Eco_Any              self)
{
    struct Eco_Type*  type;
    
    if (target == NULL) return false;   // TODO: Do we even need this?

    type = target->type;

    if (target->type->proxy != NULL) {
        return Eco_Send_ToObject(message, link, target->type->proxy, self);
    } else {
        // TODO: Check if this is even a molecule!
        return Eco_Type_SendMessageToMolecule(message, link, self, type, (struct Eco_Molecule*) target);
    }
}
