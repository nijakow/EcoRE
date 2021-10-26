#include "clone.h"

#include <ecore/vm/core/clone.h>


bool Eco_VM_Builtin_Clone(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any                result;
    struct Eco_CloneState  state;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_CloneState_Create(&state);
    Eco_CloneState_CloneAny(&state, &result, Eco_Fiber_Peek(fiber));
    Eco_CloneState_Destroy(&state);
    Eco_Any_AssignAny(Eco_Fiber_Peek(fiber), &result);
    return true;
}
