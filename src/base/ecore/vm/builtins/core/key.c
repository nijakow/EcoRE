#include "key.h"

#include <ecore/objects/misc/key/key.h>

bool Eco_VM_Builtin_KeyAsString(struct Eco_Fiber* fiber, unsigned int args)
{
    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Fiber_SetAccu(fiber, Eco_Any_FromPointer(Eco_Key_AsString(Eco_Any_AsPointer(Eco_Fiber_Pop(fiber)))));
    return true;
}

bool Eco_VM_Builtin_Gensym(struct Eco_Fiber* fiber, unsigned int args)
{
    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, 0))
        return false;
    Eco_Fiber_SetAccu(fiber, Eco_Any_FromPointer(Eco_Key_Gensym()));
    return true;
}
