#include "flow.h"

#include <ecore/objects/vm/code/closure.h>
#include <ecore/vm/core/interpreter.h>
#include <ecore/vm/core/frame.h>


bool Eco_VM_Builtin_Value(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any*             the_any;
    struct Eco_Closure*  the_closure;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, ECO_VM_BUILTIN_INFINITE_ARGS))
        return false;

    the_any = Eco_Fiber_Nth(fiber, args);

    the_closure = (struct Eco_Closure*) Eco_Any_AsPointer(the_any);  // TODO: Checks

    Eco_Any_AssignAny(the_any, &the_closure->lexical->registers[0]);    // TODO: Store SELF in the closure!

    return Eco_Fiber_Enter(fiber, the_closure->lexical, the_closure->code, args);
}

bool Eco_VM_Builtin_Reset(struct Eco_Fiber* fiber, unsigned int args)
{
    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, 0))
        return false;
    Eco_Fiber_ResetFrame(fiber);
    return true;
}
