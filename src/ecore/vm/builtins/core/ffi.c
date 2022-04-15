#include "ffi.h"

#ifdef ECO_CONFIG_USE_FFI

#include <ecore/objects/misc/array/array.h>
#include <ecore/objects/misc/blob/blob.h>
#include <ecore/objects/vm/ffi/ffitype.h>
#include <ecore/objects/vm/ffi/ffifunc.h>
#include <ecore/vm/vm.h>


bool Eco_VM_Builtin_FFIType_GetForIndex(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIType*  type;
    Eco_Any              value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Fiber_Pop(fiber, &value);
    type = Eco_FFIType_GetForIndex(Eco_Any_AsInteger(value));
    value = Eco_Any_FromPointer(type);
    Eco_Fiber_Push(fiber, &value);
    return true;
}

bool Eco_VM_Builtin_FFIType_GetSizeInBytes(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIType*  type;
    Eco_Any              value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Fiber_Pop(fiber, &value);
    type = Eco_Any_AsPointer(value);
    value = Eco_Any_FromInteger(Eco_FFIType_SizeofCType(type));
    Eco_Fiber_Push(fiber, &value);
    return true;
}

bool Eco_VM_Builtin_FFIFunction_New(struct Eco_Fiber* fiber, unsigned int args)
{
    unsigned int         index;
    unsigned int         arg_count;
    Eco_Any              result;
    Eco_Any              return_type;
    Eco_Any              type_array;
    struct Eco_FFIType*  the_return_type;
    struct Eco_Array*    the_type_array;
    struct Eco_FFIFunc*  the_func;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    Eco_Fiber_Pop(fiber, &type_array);
    Eco_Fiber_Pop(fiber, &return_type);
    the_return_type = Eco_Any_AsPointer(return_type);
    the_type_array  = Eco_Any_AsPointer(type_array);
    arg_count       = Eco_Array_Size(the_type_array);

    struct Eco_FFIType*  the_arg_types[arg_count];

    for (index = 0; index < arg_count; index++)
        the_arg_types[index] = Eco_Any_AsPointer(*Eco_Array_At(the_type_array, index));
    the_func = Eco_FFIFunc_New(arg_count, the_return_type, the_arg_types);
    result = Eco_Any_FromPointer(the_func);
    Eco_Fiber_Push(fiber, &result);
    return true;
}

bool Eco_VM_Builtin_FFIFunction_ArgCount(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any function;
    Eco_Any count;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Fiber_Pop(fiber, &function);
    count = Eco_Any_FromInteger(Eco_FFIFunc_GetArgumentCount(Eco_Any_AsPointer(function)));
    Eco_Fiber_Push(fiber, &count);
    return true;
}

bool Eco_VM_Builtin_FFIFunction_ReturnType(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any function;
    Eco_Any result;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Fiber_Pop(fiber, &function);
    result = Eco_Any_FromPointer(Eco_FFIFunc_GetReturnType(Eco_Any_AsPointer(function)));
    Eco_Fiber_Push(fiber, &result);
    return true;
}

bool Eco_VM_Builtin_FFIFunction_ArgType(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any function;
    Eco_Any index;
    Eco_Any result;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    Eco_Fiber_Pop(fiber, &index);
    Eco_Fiber_Pop(fiber, &function);
    result = Eco_Any_FromPointer(Eco_FFIFunc_GetArgumentType(Eco_Any_AsPointer(function), Eco_Any_AsInteger(index)));
    Eco_Fiber_Push(fiber, &result);
    return true;
}

bool Eco_VM_Builtin_FFIFunction_Call(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIFunc*  ffifunc;
    struct Eco_Blob*     func;
    struct Eco_Blob*     fargs;
    Eco_Any              any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 3, 3))
        return false;
    Eco_Fiber_Pop(fiber, &any);
    fargs = Eco_Any_AsPointer(any);
    Eco_Fiber_Pop(fiber, &any);
    func = Eco_Any_AsPointer(any);
    Eco_Fiber_Pop(fiber, &any);
    ffifunc = Eco_Any_AsPointer(any);
    if (Eco_FFIFunc_Call(ffifunc, *((void**) func->bytes), (void*) fargs->bytes))
        any = fiber->vm->constants.ctrue;
    else
        any = fiber->vm->constants.cfalse;
    Eco_Fiber_Push(fiber, &any);
    return true;
}

#endif
