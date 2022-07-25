#include "ffi.h"

#ifdef ECO_CONFIG_USE_FFI

#include <ecore/objects/misc/array/array.h>
#include <ecore/objects/misc/string/string.h>
#include <ecore/objects/vm/ffi/ffitype.h>
#include <ecore/objects/vm/ffi/ffiobject.h>
#include <ecore/objects/vm/ffi/ffifunc.h>
#include <ecore/objects/vm/ffi/ffilib.h>
#include <ecore/vm/vm.h>


static bool Eco_VM_Builtin_FFIType_NewStructOrUnion(struct Eco_Fiber* fiber, unsigned int args, bool is_union)
{
    struct Eco_Array*  type_array;
    struct Eco_Array*  name_array;
    Eco_Any            any;
    unsigned int       count;
    unsigned int       index;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 2))
        return false;
    if (args == 2) {
        any        = Eco_Fiber_Pop(fiber);
        name_array = Eco_Any_AsPointer(any);
    } else {
        name_array = NULL;
    }
    any        = Eco_Fiber_Pop(fiber);
    type_array = Eco_Any_AsPointer(any);
    count      = Eco_Array_Size(type_array);
    struct Eco_FFIType* types[count];
    struct Eco_Key*     names[count];
    for (index = 0; index < count; index++)
    {
        types[index] = Eco_Any_AsPointer(*Eco_Array_At(type_array, index));
        if (name_array != NULL && index < Eco_Array_Size(name_array))
            names[index] = Eco_Any_AsPointer(*Eco_Array_At(name_array, index));
        else
            names[index] = NULL;
    }
    any = Eco_Any_FromPointer(Eco_FFIType_NewStruct(types, names, count, is_union));
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_FFIType_NewStruct(struct Eco_Fiber* fiber, unsigned int args)
{
    return Eco_VM_Builtin_FFIType_NewStructOrUnion(fiber, args, false);
}

bool Eco_VM_Builtin_FFIType_NewUnion(struct Eco_Fiber* fiber, unsigned int args)
{
    return Eco_VM_Builtin_FFIType_NewStructOrUnion(fiber, args, true);
}

bool Eco_VM_Builtin_FFIType_NewArray(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIType* type;
    unsigned int        count;
    Eco_Any             any;
    unsigned int        index;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    any   = Eco_Fiber_Pop(fiber);
    count = Eco_Any_AsInteger(any);
    any   = Eco_Fiber_Pop(fiber);
    type  = Eco_Any_AsPointer(any);
    struct Eco_FFIType* types[count];
    for (index = 0; index < count; index++)
    {
        types[index] = type;
    }
    any = Eco_Any_FromPointer(Eco_FFIType_NewStruct(types, NULL, count, false));
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_FFIType_PointerTo(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIType*  type;
    Eco_Any              any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    any  = Eco_Fiber_Pop(fiber);
    type = Eco_Any_AsPointer(any);
    any  = Eco_Any_FromPointer(Eco_FFIType_PointerTo(type));
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_FFIType_PointeeOrSelf(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIType*  type;
    Eco_Any              any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    any  = Eco_Fiber_Pop(fiber);
    type = Eco_Any_AsPointer(any);
    any  = Eco_Any_FromPointer(Eco_FFIType_PointeeOrSelf(type));
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_FFIType_GetForIndex(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIType*  type;
    Eco_Any              value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    
    value = Eco_Fiber_Pop(fiber);
    type  = Eco_FFIType_GetForIndex(Eco_Any_AsInteger(value));
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

    value = Eco_Fiber_Pop(fiber);
    type  = Eco_Any_AsPointer(value);
    value = Eco_Any_FromInteger(Eco_FFIType_SizeofCType(type));
    Eco_Fiber_Push(fiber, &value);
    return true;
}

bool Eco_VM_Builtin_FFIType_GetNameOf(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIType*  type;
    Eco_Any              index;
    Eco_Any              any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;

    index = Eco_Fiber_Pop(fiber);
    any   = Eco_Fiber_Pop(fiber);
    type  = Eco_Any_AsPointer(any);
    any   = Eco_Any_FromPointer(Eco_FFIType_NameOf_ByIndex(type, Eco_Any_AsInteger(index)));
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_FFIType_GetTypeOf(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIType*  type;
    Eco_Any              index;
    Eco_Any              any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;

    index = Eco_Fiber_Pop(fiber);
    any   = Eco_Fiber_Pop(fiber);
    type  = Eco_Any_AsPointer(any);
    any   = Eco_Any_FromPointer(Eco_FFIType_TypeOf_ByIndex(type, Eco_Any_AsInteger(index)));
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_FFIType_GetOffsetOf(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIType*  type;
    Eco_Any              index;
    Eco_Any              any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;

    index = Eco_Fiber_Pop(fiber);
    any   = Eco_Fiber_Pop(fiber);
    type  = Eco_Any_AsPointer(any);
    any   = Eco_Any_FromInteger(Eco_FFIType_OffsetOf_ByIndex(type, Eco_Any_AsInteger(index)));
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_FFIType_GetMemberCount(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIType*  type;
    Eco_Any              any;
    unsigned int         count;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;

    any   = Eco_Fiber_Pop(fiber);
    type  = Eco_Any_AsPointer(any);
    count = Eco_FFIType_GetStructMemberCount(type);
    any   = Eco_Any_FromInteger(count);
    Eco_Fiber_Push(fiber, &any);
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

    type_array      = Eco_Fiber_Pop(fiber);
    return_type     = Eco_Fiber_Pop(fiber);
    the_return_type = Eco_Any_AsPointer(return_type);
    the_type_array  = Eco_Any_AsPointer(type_array);
    arg_count       = Eco_Array_Size(the_type_array);

    struct Eco_FFIType*  the_arg_types[arg_count];

    for (index = 0; index < arg_count; index++)
        the_arg_types[index] = Eco_Any_AsPointer(*Eco_Array_At(the_type_array, index));
    the_func = Eco_FFIFunc_New(arg_count, the_return_type, the_arg_types);
    result   = Eco_Any_FromPointer(the_func);
    Eco_Fiber_Push(fiber, &result);
    return true;
}

bool Eco_VM_Builtin_FFIFunction_ArgCount(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any function;
    Eco_Any count;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;

    function = Eco_Fiber_Pop(fiber);
    count    = Eco_Any_FromInteger(Eco_FFIFunc_GetArgumentCount(Eco_Any_AsPointer(function)));
    Eco_Fiber_Push(fiber, &count);
    return true;
}

bool Eco_VM_Builtin_FFIFunction_ReturnType(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any function;
    Eco_Any result;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;

    function = Eco_Fiber_Pop(fiber);
    result   = Eco_Any_FromPointer(Eco_FFIFunc_GetReturnType(Eco_Any_AsPointer(function)));
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

    index    = Eco_Fiber_Pop(fiber);
    function = Eco_Fiber_Pop(fiber);
    result   = Eco_Any_FromPointer(Eco_FFIFunc_GetArgumentType(Eco_Any_AsPointer(function), Eco_Any_AsInteger(index)));
    Eco_Fiber_Push(fiber, &result);
    return true;
}

bool Eco_VM_Builtin_FFIFunction_Call(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIFunc*    ffifunc;
    struct Eco_FFIObject*  func;
    struct Eco_FFIObject*  fargs;
    Eco_Any                any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 3, 3))
        return false;

    any     = Eco_Fiber_Pop(fiber);
    fargs   = Eco_Any_AsPointer(any);
    any     = Eco_Fiber_Pop(fiber);
    func    = Eco_Any_AsPointer(any);
    any     = Eco_Fiber_Pop(fiber);
    ffifunc = Eco_Any_AsPointer(any);
    if (Eco_FFIFunc_Call(ffifunc, *((void**) Eco_FFIObject_GetBytes(func)), Eco_FFIObject_GetBytes(fargs)))
        any = fiber->vm->constants.ctrue;
    else
        any = fiber->vm->constants.cfalse;
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_FFIFunction_EcoCall(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIFunc*    ffifunc;
    struct Eco_FFIObject*  func;
    Eco_Any                result;
    unsigned int           index;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, ECO_VM_BUILTIN_INFINITE_ARGS))
        return false;

    ffifunc = Eco_Any_AsPointer(*Eco_Fiber_Nth(fiber, args));
    func    = Eco_Any_AsPointer(*Eco_Fiber_Nth(fiber, args - 1));
    if (!Eco_FFIFunc_EcoCall(ffifunc, *((void**) Eco_FFIObject_GetBytes(func)), Eco_Fiber_Nth(fiber, args - 2), args - 2, &result))
        return false;
    for (index = 0; index < args; index++)
        Eco_Fiber_Pop(fiber);
    Eco_Fiber_Push(fiber, &result);
    return true;
}

bool Eco_VM_Builtin_FFIObject_Alloc(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIType*    type;
    Eco_Any                any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    
    any  = Eco_Fiber_Pop(fiber);
    type = Eco_Any_AsPointer(any);
    any  = Eco_Any_FromPointer(Eco_FFIObject_New(type, NULL, Eco_FFIType_SizeofCType(type)));
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_FFIObject_GetType(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIObject*  object;
    Eco_Any                any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;

    any    = Eco_Fiber_Pop(fiber);
    object = Eco_Any_AsPointer(any);
    any    = Eco_Any_FromPointer(Eco_FFIObject_GetFFIType(object));
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_FFIObject_GetSize(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIObject*  object;
    Eco_Any                any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;

    any    = Eco_Fiber_Pop(fiber);
    object = Eco_Any_AsPointer(any);
    any    = Eco_Any_FromInteger(Eco_FFIObject_GetSize(object));
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_FFIObject_AssignNull(struct Eco_Fiber* fiber, unsigned int args)
{
    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;

    Eco_FFIObject_AssignNull(Eco_Any_AsPointer(*Eco_Fiber_Peek(fiber)));
    return true;
}

bool Eco_VM_Builtin_FFIObject_Address(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIObject*  object;
    struct Eco_FFIObject*  pointer;
    Eco_Any                any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;

    any     = Eco_Fiber_Pop(fiber);
    object  = Eco_Any_AsPointer(any);
    pointer = Eco_FFIObject_Address(object);
    any     = Eco_Any_FromPointer(pointer);
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_FFIObject_Fetch(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIObject*  object;
    Eco_Any                any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;

    any     = Eco_Fiber_Pop(fiber);
    object  = Eco_Any_AsPointer(any);
    any     = Eco_FFIObject_Fetch(object);
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_FFIObject_Store(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIObject*  object;
    Eco_Any                any;
    Eco_Any                value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;

    value  = Eco_Fiber_Pop(fiber);
    any    = Eco_Fiber_Pop(fiber);
    object = Eco_Any_AsPointer(any);
    Eco_FFIObject_Store(object, value);
    any    = Eco_Any_FromPointer(object);
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_FFIObject_FetchWithOffset(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIObject*  object;
    struct Eco_FFIType*    type;
    Eco_Any                any;
    unsigned int           offset;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 3, 3))
        return false;

    any    = Eco_Fiber_Pop(fiber);
    offset = Eco_Any_AsInteger(any);
    any    = Eco_Fiber_Pop(fiber);
    type   = Eco_Any_AsPointer(any);
    any    = Eco_Fiber_Pop(fiber);
    object = Eco_Any_AsPointer(any);
    any    = Eco_FFIObject_FetchWithOffset(object, type, offset);
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_FFIObject_StoreWithOffset(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIObject*  object;
    struct Eco_FFIType*    type;
    Eco_Any                any;
    Eco_Any                value;
    unsigned int           offset;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 4, 4))
        return false;

    value  = Eco_Fiber_Pop(fiber);
    any    = Eco_Fiber_Pop(fiber);
    offset = Eco_Any_AsInteger(any);
    any    = Eco_Fiber_Pop(fiber);
    type   = Eco_Any_AsPointer(any);
    any    = Eco_Fiber_Pop(fiber);
    object = Eco_Any_AsPointer(any);
    Eco_FFIObject_StoreWithOffset(object, type, offset, value);
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_FFIObject_Access(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIObject*  object;
    struct Eco_FFIObject*  pointer;
    Eco_Any                any;
    unsigned int           index;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;

    any     = Eco_Fiber_Pop(fiber);
    index   = Eco_Any_AsInteger(any);
    any     = Eco_Fiber_Pop(fiber);
    object  = Eco_Any_AsPointer(any);
    pointer = Eco_FFIObject_Access(object, index);
    any     = Eco_Any_FromPointer(pointer);
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_FFIObject_Cast(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIObject*  object;
    struct Eco_FFIObject*  cast;
    struct Eco_FFIType*    type;
    Eco_Any                any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;

    any    = Eco_Fiber_Pop(fiber);
    type   = Eco_Any_AsPointer(any);
    any    = Eco_Fiber_Pop(fiber);
    object = Eco_Any_AsPointer(any);
    cast   = Eco_FFIObject_Cast(object, type);
    any    = Eco_Any_FromPointer(cast);
    Eco_Fiber_Push(fiber, &any);
    return true;
}

#ifdef ECO_CONFIG_USE_DLOPEN
bool Eco_VM_Builtin_FFIObjectDLOpen(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_String*    string;
    struct Eco_FFIObject* object;
    Eco_Any               any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;

    any    = Eco_Fiber_Pop(fiber);
    string = Eco_Any_AsPointer(any);
    object = Eco_FFIObject_DLOpen(string->bytes);
    if (object != NULL)
        any = Eco_Any_FromPointer(object);
    else
        any = fiber->vm->constants.cfalse;
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_FFIObjectDLSym(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_Key*        key;
    struct Eco_FFIObject*  object;
    Eco_Any                any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 2))
        return false;

    any = Eco_Fiber_Pop(fiber);
    key = Eco_Any_AsPointer(any);
    if (args == 1) {
        object = Eco_FFIObject_DLSym(NULL, key->name);
    } else {
        any    = Eco_Fiber_Pop(fiber);
        object = Eco_Any_AsPointer(any);
        object = Eco_FFIObject_DLSym(*((void**) Eco_FFIObject_GetBytes(object)), key->name);
    }
    if (object != NULL)
        any = Eco_Any_FromPointer(object);
    else
        any = fiber->vm->constants.cfalse;
    Eco_Fiber_Push(fiber, &any);
    return true;
}
#endif

bool Eco_VM_Builtin_FFILib_Open(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any             any;
    struct Eco_String*  path;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;

    any  = Eco_Fiber_Pop(fiber);
    path = Eco_Any_AsPointer(any);
    any  = Eco_Any_FromPointer(Eco_FFILib_New(path->bytes));
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_FFILib_At(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any             any;
    struct Eco_FFILib*  lib;
    struct Eco_Key*     key;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;

    any = Eco_Fiber_Pop(fiber);
    key = Eco_Any_AsPointer(any);
    any = Eco_Fiber_Pop(fiber);
    lib = Eco_Any_AsPointer(any);
    if (!Eco_FFILib_At(lib, key, &any))
        any = Eco_Any_FromPointer(lib);
    Eco_Fiber_Push(fiber, &any);
    return true;
}

#endif
