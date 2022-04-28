#include "object.h"
#include "send.h"

#include <ecore/objects/base/type.h>
#include <ecore/objects/base/type/sending.h>
#include <ecore/vm/core/send.h>
#include <ecore/objects/vm/interface/interface.h>

bool Eco_Object_Send(struct Eco_Message*  message,
                     struct Eco_Object*   target,
                     Eco_Any              self,
                     bool                 delegated_send)
{
    struct Eco_Type*  type;
    
    type = target->type;

    if (target->type->proxy != NULL) {
        return Eco_Send_ToObject(message, target->type->proxy, self, delegated_send);
    } else {
        // TODO: Check if this is even a molecule!
        return Eco_Type_SendMessageToMolecule(message, self, type, (struct Eco_Molecule*) target, delegated_send);
    }
}
