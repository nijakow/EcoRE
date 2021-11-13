#include "reflection.h"

#include <ecore/base/extra.h>

bool Eco_VM_Builtin_GetType(struct Eco_Fiber* fiber, unsigned int args)
{
    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    Eco_Any_AssignPointer(Eco_Fiber_Peek(fiber),
                          (struct Eco_Object*) Eco_Any_GetType(Eco_Fiber_Peek(fiber)));
    return true;
}
