#include "is.h"

#include <ecore/vm/vm.h>
#include <ecore/vm/fiber/stackops.h>
#include <ecore/io/logging/log.h>


bool Eco_VM_Builtin_Is(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  arg;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    Eco_Fiber_Pop(fiber, &arg);
    if (*Eco_Fiber_Peek(fiber) == arg) {
        Eco_Any_AssignAny(Eco_Fiber_Peek(fiber), &fiber->vm->constants.ctrue);
    } else {
        Eco_Any_AssignAny(Eco_Fiber_Peek(fiber), &fiber->vm->constants.cfalse);
    }
    return true;
}

bool Eco_VM_Builtin_CharAsInt(struct Eco_Fiber* fiber, unsigned int args)
{
    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Any_AssignInteger(Eco_Fiber_Peek(fiber), Eco_Any_AsCharacter(*Eco_Fiber_Peek(fiber)));
    return true;
}

bool Eco_VM_Builtin_IntAsChar(struct Eco_Fiber* fiber, unsigned int args)
{
    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Any_AssignCharacter(Eco_Fiber_Peek(fiber), Eco_Any_AsInteger(*Eco_Fiber_Peek(fiber)));
    return true;
}
