#include "is.h"

#include <ecore/vm/vm.h>
#include <ecore/vm/fiber/stackops.h>
#include <ecore/io/logging/log.h>


bool Eco_VM_Builtin_Is(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  arg1;
    Eco_Any  arg2;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    arg2 = Eco_Fiber_Pop(fiber);
    arg1 = Eco_Fiber_Pop(fiber);
    if (Eco_Any_Equals(arg1, arg2)) {
        Eco_Fiber_SetAccu(fiber, fiber->vm->constants.ctrue);
    } else {
        Eco_Fiber_SetAccu(fiber, fiber->vm->constants.cfalse);
    }
    return true;
}

bool Eco_VM_Builtin_CharAsInt(struct Eco_Fiber* fiber, unsigned int args)
{
    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Fiber_SetAccu(fiber, Eco_Any_FromInteger(Eco_Any_AsCharacter(Eco_Fiber_Pop(fiber))));
    return true;
}

bool Eco_VM_Builtin_IntAsChar(struct Eco_Fiber* fiber, unsigned int args)
{
    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Fiber_SetAccu(fiber, Eco_Any_FromCharacter(Eco_Any_AsInteger(Eco_Fiber_Pop(fiber))));
    return true;
}
