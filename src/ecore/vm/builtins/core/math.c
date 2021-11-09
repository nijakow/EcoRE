#include "math.h"

#include <ecore/vm/vm.h>


bool Eco_VM_Builtin_AddIntegers(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Integer  value;
    Eco_Any      any;

    value = 0;
    while (args > 0)
    {
        Eco_Fiber_Pop(fiber, &any);
        if (!Eco_Any_IsInteger(&any)) {
            // TODO: Error
            return false;
        }
        value += Eco_Any_AsInteger(&any);
        args--;
    }

    Eco_Any_AssignInteger(&any, value);
    Eco_Fiber_Push(fiber, &any);

    return true;
}

bool Eco_VM_Builtin_Add(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Integer   value;
    Eco_Floating  floating;
    Eco_Any       any;

    value = 0;
    while (args --> 0)
    {
        Eco_Fiber_Pop(fiber, &any);
        if (Eco_Any_IsInteger(&any)) value += Eco_Any_AsInteger(&any);
        else if (Eco_Any_IsFloating(&any)) {
            floating = value + Eco_Any_AsFloating(&any);
            while (args --> 0)
            {
                Eco_Fiber_Pop(fiber, &any);
                if (Eco_Any_IsInteger(&any)) floating += Eco_Any_AsInteger(&any);
                else if (Eco_Any_IsFloating(&any)) floating += Eco_Any_AsFloating(&any);
                else goto error;
            }
            Eco_Any_AssignFloating(&any, floating);
            goto success;
        }
        else goto error;
    }
    Eco_Any_AssignInteger(&any, value);

    success:
    Eco_Fiber_Push(fiber, &any);
    return true;

    error:
    // TODO: Set error type
    return false;
}

bool Eco_VM_Builtin_Subtract2(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any       arg1;
    Eco_Any       arg2;
    Eco_Any       result;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;

    /*
     * To avoid "uninitialized variable" warnings
     */
    Eco_Any_Initialize(&result);

    Eco_Fiber_Pop(fiber, &arg2);
    Eco_Fiber_Pop(fiber, &arg1);

    if (Eco_Any_IsInteger(&arg1)) {
        if (Eco_Any_IsInteger(&arg2)) {
            Eco_Any_AssignInteger(&result, Eco_Any_AsInteger(&arg1) - Eco_Any_AsInteger(&arg2));
        } else if (Eco_Any_IsFloating(&arg2)) {
            Eco_Any_AssignFloating(&result, Eco_Any_AsInteger(&arg1) - Eco_Any_AsFloating(&arg2));
        } else {
            goto error;
        }
    } else if (Eco_Any_IsFloating(&arg1)) {
        if (Eco_Any_IsInteger(&arg2)) {
            Eco_Any_AssignFloating(&result, Eco_Any_AsFloating(&arg1) - Eco_Any_AsInteger(&arg2));
        } else if (Eco_Any_IsFloating(&arg2)) {
            Eco_Any_AssignFloating(&result, Eco_Any_AsFloating(&arg1) - Eco_Any_AsFloating(&arg2));
        } else {
            goto error;
        }
    } else {
        goto error;
    }

    Eco_Fiber_Push(fiber, &result);
    return true;
  error:
    // TODO: Set error type
    return false;
}

bool Eco_VM_Builtin_Multiply2(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any       arg1;
    Eco_Any       arg2;
    Eco_Any       result;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;

    /*
     * To avoid "uninitialized variable" warnings
     */
    Eco_Any_Initialize(&result);

    Eco_Fiber_Pop(fiber, &arg2);
    Eco_Fiber_Pop(fiber, &arg1);

    if (Eco_Any_IsInteger(&arg1)) {
        if (Eco_Any_IsInteger(&arg2)) {
            Eco_Any_AssignInteger(&result, Eco_Any_AsInteger(&arg1) * Eco_Any_AsInteger(&arg2));
        } else if (Eco_Any_IsFloating(&arg2)) {
            Eco_Any_AssignFloating(&result, Eco_Any_AsInteger(&arg1) * Eco_Any_AsFloating(&arg2));
        } else {
            goto error;
        }
    } else if (Eco_Any_IsFloating(&arg1)) {
        if (Eco_Any_IsInteger(&arg2)) {
            Eco_Any_AssignFloating(&result, Eco_Any_AsFloating(&arg1) * Eco_Any_AsInteger(&arg2));
        } else if (Eco_Any_IsFloating(&arg2)) {
            Eco_Any_AssignFloating(&result, Eco_Any_AsFloating(&arg1) * Eco_Any_AsFloating(&arg2));
        } else {
            goto error;
        }
    } else {
        goto error;
    }

    Eco_Fiber_Push(fiber, &result);
    return true;
  error:
    // TODO: Set error type
    return false;
}

bool Eco_VM_Builtin_Divide2(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any       arg1;
    Eco_Any       arg2;
    Eco_Any       result;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;

    /*
     * To avoid "uninitialized variable" warnings
     */
    Eco_Any_Initialize(&result);

    Eco_Fiber_Pop(fiber, &arg2);
    Eco_Fiber_Pop(fiber, &arg1);

    if (Eco_Any_IsInteger(&arg1)) {
        if (Eco_Any_IsInteger(&arg2)) {
            Eco_Any_AssignInteger(&result, Eco_Any_AsInteger(&arg1) / Eco_Any_AsInteger(&arg2));
        } else if (Eco_Any_IsFloating(&arg2)) {
            Eco_Any_AssignFloating(&result, Eco_Any_AsInteger(&arg1) / Eco_Any_AsFloating(&arg2));
        } else {
            goto error;
        }
    } else if (Eco_Any_IsFloating(&arg1)) {
        if (Eco_Any_IsInteger(&arg2)) {
            Eco_Any_AssignFloating(&result, Eco_Any_AsFloating(&arg1) / Eco_Any_AsInteger(&arg2));
        } else if (Eco_Any_IsFloating(&arg2)) {
            Eco_Any_AssignFloating(&result, Eco_Any_AsFloating(&arg1) / Eco_Any_AsFloating(&arg2));
        } else {
            goto error;
        }
    } else {
        goto error;
    }

    Eco_Fiber_Push(fiber, &result);
    return true;
  error:
    // TODO: Set error type
    return false;
}

bool Eco_VM_Builtin_Modulo2(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any       arg1;
    Eco_Any       arg2;
    Eco_Any       result;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;

    /*
     * To avoid "uninitialized variable" warnings
     */
    Eco_Any_Initialize(&result);

    Eco_Fiber_Pop(fiber, &arg2);
    Eco_Fiber_Pop(fiber, &arg1);

    if (Eco_Any_IsInteger(&arg1) && Eco_Any_IsInteger(&arg2)) {
        Eco_Any_AssignInteger(&result, Eco_Any_AsInteger(&arg1) % Eco_Any_AsInteger(&arg2));
        Eco_Fiber_Push(fiber, &result);
        return true;
    } else {
        // TODO: Set error type
        return false;
    }
}

bool Eco_VM_Builtin_Less2(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any       arg1;
    Eco_Any       arg2;
    Eco_Any       result;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;

    /*
     * To avoid "uninitialized variable" warnings
     */
    Eco_Any_Initialize(&result);

    Eco_Fiber_Pop(fiber, &arg2);
    Eco_Fiber_Pop(fiber, &arg1);

    if (Eco_Any_IsInteger(&arg1)) {
        if (Eco_Any_IsInteger(&arg2)) {
            Eco_Any_AssignAny(&result, Eco_Any_AsInteger(&arg1) < Eco_Any_AsInteger(&arg2) ? &fiber->vm->constants.ctrue : &fiber->vm->constants.cfalse);
        } else if (Eco_Any_IsFloating(&arg2)) {
            Eco_Any_AssignAny(&result, Eco_Any_AsInteger(&arg1) < Eco_Any_AsFloating(&arg2) ? &fiber->vm->constants.ctrue : &fiber->vm->constants.cfalse);
        } else {
            goto error;
        }
    } else if (Eco_Any_IsFloating(&arg1)) {
        if (Eco_Any_IsInteger(&arg2)) {
            Eco_Any_AssignAny(&result, Eco_Any_AsFloating(&arg1) < Eco_Any_AsInteger(&arg2) ? &fiber->vm->constants.ctrue : &fiber->vm->constants.cfalse);
        } else if (Eco_Any_IsFloating(&arg2)) {
            Eco_Any_AssignAny(&result, Eco_Any_AsFloating(&arg1) < Eco_Any_AsFloating(&arg2) ? &fiber->vm->constants.ctrue : &fiber->vm->constants.cfalse);
        } else {
            goto error;
        }
    } else {
        goto error;
    }

    Eco_Fiber_Push(fiber, &result);
    return true;
  error:
    // TODO: Set error type
    return false;
}
