#include "ffi.h"

#include <ecore/objects/vm/ffi/ffitype.h>


bool Eco_VM_Builtin_FFIType_GetForIndex(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIType*  type;
    Eco_Any              value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, ECO_VM_BUILTIN_INFINITE_ARGS))
        return false;
    Eco_Fiber_Pop(fiber, &value);
    type = Eco_FFIType_GetForIndex(Eco_Any_AsInteger(&value));
    Eco_Any_AssignPointer(&value, type);
    Eco_Fiber_Push(fiber, &value);
    return true;
}

bool Eco_VM_Builtin_FFIType_GetSizeInBytes(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIType*  type;
    Eco_Any              value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, ECO_VM_BUILTIN_INFINITE_ARGS))
        return false;
    Eco_Fiber_Pop(fiber, &value);
    type = Eco_Any_AsPointer(&value);
    Eco_Any_AssignInteger(&value, Eco_FFIType_SizeofCType(type));
    Eco_Fiber_Push(fiber, &value);
    return true;
}
