#include "code.h"

#include <ecore/objects/misc/array/array.h>
#include <ecore/objects/misc/blob/blob.h>
#include <ecore/objects/vm/code/code.h>
#include <ecore/vm/core/frame.h>
#include <ecore/vm/core/interpreter.h>


bool Eco_VM_Builtin_Code_New(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_Code*  code;
    Eco_Any           result;
    Eco_Any           bytecodes;
    Eco_Any           constants;
    Eco_Any           registers;
    Eco_Any           fixed_args;
    Eco_Any           flags;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 5, 5))
        return false;
    flags          = Eco_Fiber_Pop(fiber);
    fixed_args     = Eco_Fiber_Pop(fiber);
    registers      = Eco_Fiber_Pop(fiber);
    constants      = Eco_Fiber_Pop(fiber);
    bytecodes      = Eco_Fiber_Pop(fiber);
    code           = Eco_Code_ConstructFromEco((struct Eco_Blob*)  Eco_Any_AsPointer(bytecodes),
                                               (struct Eco_Array*) Eco_Any_AsPointer(constants),
                                               Eco_Any_AsInteger(registers),
                                               Eco_Any_AsInteger(fixed_args),
                                               Eco_Any_AsInteger(flags) != 0);
    result         = Eco_Any_FromPointer(code);
    Eco_Fiber_Push(fiber, result);
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

    code = Eco_Any_AsPointer(Eco_Fiber_Top(fiber)->args[0]);  // TODO: Checks

    return Eco_Fiber_Enter(fiber, Eco_Fiber_Top(fiber)->myself, NULL, code, args);  // FIXME: Should MYSELF point to the code proxy?
}

bool Eco_VM_Builtin_Code_Bytecodes(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_Code*  code;
    struct Eco_Blob*  bytes;
    Eco_Any           any;
    unsigned int      i;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;

    any   = Eco_Fiber_Pop(fiber);
    code  = Eco_Any_AsPointer(any);
    bytes = Eco_Blob_New(code->bytecode_count);
    for (i = 0; i < code->bytecode_count; i++)
        Eco_Blob_AtPutInt8(bytes, i, code->bytecodes[i]);
    any = Eco_Any_FromPointer(bytes);
    Eco_Fiber_Push(fiber, any);
    return true;
}

bool Eco_VM_Builtin_Code_Constants(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_Code*   code;
    struct Eco_Array*  constants;
    Eco_Any            any;
    unsigned int       i;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;

    any       = Eco_Fiber_Pop(fiber);
    code      = Eco_Any_AsPointer(any);
    constants = Eco_Array_New(code->constant_count);
    for (i = 0; i < code->constant_count; i++)
        Eco_Array_Put(constants, i, code->constants[i]);
    any = Eco_Any_FromPointer(constants);
    Eco_Fiber_Push(fiber, any);
    return true;
}
