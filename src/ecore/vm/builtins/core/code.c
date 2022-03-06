#include "code.h"

#include <ecore/objects/vm/code/code.h>
#include <ecore/vm/core/frame.h>
#include <ecore/vm/core/interpreter.h>


bool Eco_VM_Builtin_Code_New(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_Code*  code;
    Eco_Any           result;
    Eco_Any           bytecodes;
    Eco_Any           constants;
    Eco_Any           code_instances;
    Eco_Any           registers;
    Eco_Any           fixed_args;
    Eco_Any           flags;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 6, 6))
        return false;
    Eco_Fiber_Pop(fiber, &flags);
    Eco_Fiber_Pop(fiber, &fixed_args);
    Eco_Fiber_Pop(fiber, &registers);
    Eco_Fiber_Pop(fiber, &code_instances);
    Eco_Fiber_Pop(fiber, &constants);
    Eco_Fiber_Pop(fiber, &bytecodes);
    code = Eco_Code_ConstructFromEco((struct Eco_Blob*)  Eco_Any_AsPointer(&bytecodes),
                                     (struct Eco_Array*) Eco_Any_AsPointer(&constants),
                                     (struct Eco_Array*) Eco_Any_AsPointer(&code_instances),
                                     Eco_Any_AsInteger(&registers),
                                     Eco_Any_AsInteger(&fixed_args),
                                     Eco_Any_AsInteger(&flags) != 0);
    Eco_Any_AssignPointer(&result, (struct Eco_Object*) code);
    Eco_Fiber_Push(fiber, &result);
    return true;
}

bool Eco_VM_Builtin_Code_Value(struct Eco_Fiber* fiber, unsigned int args)
{
    /*
     * NOTE: This builtin extracts the code to be run from regs[0].
     *       Therefore, it can not be used to run every builtin.
     *       A mechanism for that must be implemented on a higher level.
     */
    struct Eco_Code*  code;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, ECO_VM_BUILTIN_INFINITE_ARGS))
        return false;

    code = (struct Eco_Code*) Eco_Any_AsPointer(&Eco_Fiber_Top(fiber)->registers[0]);  // TODO: Checks

    return Eco_Fiber_Enter(fiber, NULL, code, args);
}
