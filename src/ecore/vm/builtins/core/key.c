#include "key.h"

#include <ecore/objects/misc/key/key.h>

bool Eco_VM_Builtin_KeyAsString(struct Eco_Fiber* fiber, unsigned int args)
{
    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Any_AssignPointer(Eco_Fiber_Peek(fiber), (struct Eco_Object*) Eco_Key_AsString((struct Eco_Key*) Eco_Any_AsPointer(*Eco_Fiber_Peek(fiber))));
    return true;
}
