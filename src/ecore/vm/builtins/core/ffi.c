#include "ffi.h"

#ifdef ECO_CONFIG_USE_FFI

#include <ecore/objects/misc/array/array.h>
#include <ecore/objects/misc/string/string.h>
#include <ecore/objects/vm/ffi/ffitype.h>
#include <ecore/objects/vm/ffi/ffiobject.h>
#include <ecore/objects/vm/ffi/ffifunc.h>
#include <ecore/vm/vm.h>


bool Eco_VM_Builtin_FFIType_NewStruct(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_Array*  type_array;
    struct Eco_Array*  name_array;
    Eco_Any            any;
    unsigned int       count;
    unsigned int       index;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 2))
        return false;
    if (args == 2) {
        Eco_Fiber_Pop(fiber, &any);
        name_array = Eco_Any_AsPointer(any);
    } else {
        name_array = NULL;
    }
    Eco_Fiber_Pop(fiber, &any);
    type_array = Eco_Any_AsPointer(any);
    count = Eco_Array_Size(type_array);
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
    any = Eco_Any_FromPointer(Eco_FFIType_NewStruct(types, names, count));
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_FFIType_PointerTo(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIType*  type;
    Eco_Any              any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Fiber_Pop(fiber, &any);
    type = Eco_Any_AsPointer(any);
    any = Eco_Any_FromPointer(Eco_FFIType_PointerTo(type));
    Eco_Fiber_Push(fiber, &any);
    return true;
}

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

bool Eco_VM_Builtin_FFIType_GetOffsetOf(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIType*  type;
    Eco_Any              index;
    Eco_Any              any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    Eco_Fiber_Pop(fiber, &index);
    Eco_Fiber_Pop(fiber, &any);
    type = Eco_Any_AsPointer(any);
    if (Eco_Any_IsInteger(index)) {
        any = Eco_Any_FromInteger(Eco_FFIType_OffsetOf_ByIndex(type, Eco_Any_AsInteger(index)));
    } else if (Eco_Any_IsPointer(index)) {
        any = Eco_Any_FromInteger(Eco_FFIType_OffsetOf_ByName(type, Eco_Any_AsPointer(index)));
    } else {
        // TODO: How do the stack pops affect the failing builtin?
        return false;
    }
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
    struct Eco_FFIFunc*    ffifunc;
    struct Eco_FFIObject*  func;
    struct Eco_FFIObject*  fargs;
    Eco_Any                any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 3, 3))
        return false;
    Eco_Fiber_Pop(fiber, &any);
    fargs = Eco_Any_AsPointer(any);
    Eco_Fiber_Pop(fiber, &any);
    func = Eco_Any_AsPointer(any);
    Eco_Fiber_Pop(fiber, &any);
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
    Eco_Any                any;
    Eco_Any                result;
    unsigned int           index;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, ECO_VM_BUILTIN_INFINITE_ARGS))
        return false;
    ffifunc = Eco_Any_AsPointer(*Eco_Fiber_Nth(fiber, args));
    func    = Eco_Any_AsPointer(*Eco_Fiber_Nth(fiber, args - 1));
    if (!Eco_FFIFunc_EcoCall(ffifunc, *((void**) Eco_FFIObject_GetBytes(func)), Eco_Fiber_Nth(fiber, args - 2), args - 2, &result))
        return false;
    for (index = 0; index < args; index++)
        Eco_Fiber_Pop(fiber, &any);
    Eco_Fiber_Push(fiber, &result);
    return true;
}

bool Eco_VM_Builtin_FFIObject_Alloc(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIType*    type;
    Eco_Any                any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Fiber_Pop(fiber, &any);
    type = Eco_Any_AsPointer(any);
    any = Eco_Any_FromPointer(Eco_FFIObject_New(type, NULL, Eco_FFIType_SizeofCType(type)));
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_FFIObject_GetType(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIObject*  object;
    Eco_Any                any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Fiber_Pop(fiber, &any);
    object = Eco_Any_AsPointer(any);
    any = Eco_Any_FromPointer(Eco_FFIObject_GetFFIType(object));
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_FFIObject_GetSize(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_FFIObject*  object;
    Eco_Any                any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Fiber_Pop(fiber, &any);
    object = Eco_Any_AsPointer(any);
    any = Eco_Any_FromInteger(Eco_FFIObject_GetSize(object));
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
    Eco_Fiber_Pop(fiber, &any);
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
    Eco_Fiber_Pop(fiber, &any);
    key = Eco_Any_AsPointer(any);
    if (args == 1) {
        object = Eco_FFIObject_DLSym(NULL, key->name);
    } else {
        Eco_Fiber_Pop(fiber, &any);
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


#endif
