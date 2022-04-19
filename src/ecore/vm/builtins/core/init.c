#include "init.h"

#include <ecore/objects/base/type.h>
#include <ecore/objects/misc/string/string.h>
#include <ecore/objects/vm/interface/interface.h>
#include <ecore/vm/vm.h>
#include <ecore/vm/fiber/stackops.h>

/*
 * TODO: Include headers for this!
 */
extern struct Eco_Type*  Eco_Array_TYPE;
extern struct Eco_Type*  Eco_Blob_TYPE;
extern struct Eco_Type*  Eco_Code_TYPE;
extern struct Eco_Type*  Eco_Integer_TYPE;
extern struct Eco_Type*  Eco_Character_TYPE;
extern struct Eco_Type*  Eco_Closure_TYPE;
extern struct Eco_Type*  Eco_Key_TYPE;
extern struct Eco_Type*  Eco_String_TYPE;
extern struct Eco_Type*  Eco_Interface_TYPE;
extern struct Eco_Type*  Eco_FFIType_TYPE;
extern struct Eco_Type*  Eco_FFIObject_TYPE;
extern struct Eco_Type*  Eco_FFIFunc_TYPE;

bool Eco_VM_Builtin_GetVersionString(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, 0))
        return false;
    any = Eco_Any_FromPointer(Eco_String_New(ECO_VERSION_STRING));
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_SetLobby(struct Eco_Fiber* fiber, unsigned int args)
{
    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Any_AssignAny(&fiber->vm->constants.lobby, Eco_Fiber_Peek(fiber));
    return true;
}

bool Eco_VM_Builtin_SetTrue(struct Eco_Fiber* fiber, unsigned int args)
{
    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Any_AssignAny(&fiber->vm->constants.ctrue, Eco_Fiber_Peek(fiber));
    return true;
}

bool Eco_VM_Builtin_SetFalse(struct Eco_Fiber* fiber, unsigned int args)
{
    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Any_AssignAny(&fiber->vm->constants.cfalse, Eco_Fiber_Peek(fiber));
    return true;
}

bool Eco_VM_Builtin_SetProxy(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  proxy;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    Eco_Fiber_Pop(fiber, &proxy);
    ((struct Eco_Type*) Eco_Any_AsPointer(*Eco_Fiber_Peek(fiber)))->proxy = Eco_Any_AsPointer(proxy);
    return true;
}

bool Eco_VM_Builtin_GetArrayType(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, 0))
        return false;
    value = Eco_Any_FromPointer(Eco_Array_TYPE);
    Eco_Fiber_Push(fiber, &value);
    return true;
}

bool Eco_VM_Builtin_GetBlobType(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, 0))
        return false;
    value = Eco_Any_FromPointer(Eco_Blob_TYPE);
    Eco_Fiber_Push(fiber, &value);
    return true;
}

bool Eco_VM_Builtin_GetCodeType(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, 0))
        return false;
    value = Eco_Any_FromPointer(Eco_Code_TYPE);
    Eco_Fiber_Push(fiber, &value);
    return true;
}

bool Eco_VM_Builtin_GetIntegerType(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, 0))
        return false;
    value = Eco_Any_FromPointer(Eco_Integer_TYPE);
    Eco_Fiber_Push(fiber, &value);
    return true;
}

bool Eco_VM_Builtin_GetCharacterType(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, 0))
        return false;
    value = Eco_Any_FromPointer(Eco_Character_TYPE);
    Eco_Fiber_Push(fiber, &value);
    return true;
}

bool Eco_VM_Builtin_GetBlockType(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, 0))
        return false;
    value = Eco_Any_FromPointer(Eco_Closure_TYPE);
    Eco_Fiber_Push(fiber, &value);
    return true;
}

bool Eco_VM_Builtin_GetKeyType(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, 0))
        return false;
    value = Eco_Any_FromPointer(Eco_Key_TYPE);
    Eco_Fiber_Push(fiber, &value);
    return true;
}

bool Eco_VM_Builtin_GetStringType(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, 0))
        return false;
    value = Eco_Any_FromPointer(Eco_String_TYPE);
    Eco_Fiber_Push(fiber, &value);
    return true;
}

bool Eco_VM_Builtin_GetInterfaceType(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, 0))
        return false;
    value = Eco_Any_FromPointer(Eco_Interface_TYPE);
    Eco_Fiber_Push(fiber, &value);
    return true;
}

bool Eco_VM_Builtin_GetDefaultInterface(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, 0))
        return false;
    value = Eco_Any_FromPointer(Eco_Interface_GetDefaultInterface());
    Eco_Fiber_Push(fiber, &value);
    return true;
}

bool Eco_VM_Builtin_GetFFITypeType(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, 0))
        return false;
    value = Eco_Any_FromPointer(Eco_FFIType_TYPE);
    Eco_Fiber_Push(fiber, &value);
    return true;
}

bool Eco_VM_Builtin_GetFFIObjectType(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, 0))
        return false;
    value = Eco_Any_FromPointer(Eco_FFIObject_TYPE);
    Eco_Fiber_Push(fiber, &value);
    return true;
}

bool Eco_VM_Builtin_GetFFIFuncType(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, 0))
        return false;
    value = Eco_Any_FromPointer(Eco_FFIFunc_TYPE);
    Eco_Fiber_Push(fiber, &value);
    return true;
}
