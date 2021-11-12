#include "is.h"

#include <ecore/vm/vm.h>


bool Eco_VM_Builtin_Is(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any                arg;
    Eco_Any                result;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    Eco_Any_Initialize(&result);
    Eco_Fiber_Pop(fiber, &arg);
    if (*Eco_Fiber_Peek(fiber) == arg) {
        Eco_Any_AssignAny(Eco_Fiber_Peek(fiber), &fiber->vm->constants.ctrue);
    } else {
        Eco_Any_AssignAny(Eco_Fiber_Peek(fiber), &fiber->vm->constants.cfalse);
    }
    Eco_Any_AssignAny(Eco_Fiber_Peek(fiber), &result);
    return true;
}
