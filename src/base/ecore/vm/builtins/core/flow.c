#include "flow.h"

#include <ecore/objects/vm/code/closure.h>
#include <ecore/vm/core/interpreter.h>
#include <ecore/vm/core/frame.h>
#include <ecore/vm/fiber/fiber.h>
#include <ecore/vm/fiber/stackops.h>


bool Eco_VM_Builtin_Myself(struct Eco_Fiber* fiber, unsigned int args)
{
    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, 0))
        return false;
    Eco_Fiber_SetAccu(fiber, Eco_Fiber_Top(fiber)->myself);
    return true;
}

bool Eco_VM_Builtin_Value(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any*             the_any;
    struct Eco_Closure*  the_closure;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, ECO_VM_BUILTIN_INFINITE_ARGS))
        return false;

    the_any = Eco_Fiber_Nth(fiber, args);

    the_closure = (struct Eco_Closure*) Eco_Any_AsPointer(*the_any);  // TODO: Checks

    Eco_Any_AssignAny(the_any, &the_closure->lexical->args[0]);    // TODO: Store SELF in the closure!

    return Eco_Fiber_Enter(fiber, the_closure->lexical->myself, the_closure->lexical, the_closure->code, args);
}

bool Eco_VM_Builtin_Reset(struct Eco_Fiber* fiber, unsigned int args)
{
    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, 0))
        return false;
    Eco_Fiber_ResetFrame(fiber);
    return true;
}

bool Eco_VM_Builtin_Throw(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    value = Eco_Fiber_Pop(fiber);
    Eco_Fiber_Throw(fiber, value);
    return true;
}

bool Eco_VM_Builtin_SetExceptionHandler(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_Closure*  closure;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    closure = Eco_Any_AsPointer(Eco_Fiber_Pop(fiber));    // TODO: Checks!
    Eco_Fiber_Top(fiber)->handler = closure;
    return true;
}
