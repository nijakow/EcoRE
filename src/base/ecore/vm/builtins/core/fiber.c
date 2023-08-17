#include "fiber.h"

#include <ecore/vm/core/frame.h>
#include <ecore/vm/fiber/fiber.h>
#include <ecore/vm/fiber/stackops.h>
#include <ecore/vm/vm.h>

bool Eco_VM_Builtin_StartFiber(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any            any;
    Eco_Any            lobby;
    struct Eco_Code*   code;
    struct Eco_Fiber*  new_fiber;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;

    lobby = Eco_Fiber_Pop(fiber);
    any   = Eco_Fiber_Pop(fiber);
    code  = Eco_Any_AsPointer(any);

    new_fiber = Eco_VM_SpawnCode(fiber->vm, code, &lobby, 1);

    any = Eco_Any_FromInteger(new_fiber->id);
    Eco_Fiber_Push(fiber, any);

    return true;
}


bool Eco_VM_Builtin_Yield(struct Eco_Fiber* fiber, unsigned int args)
{
    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, 0))
        return false;
    Eco_Fiber_Yield(fiber);
    return true;
}
