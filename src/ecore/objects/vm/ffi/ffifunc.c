#include "ffifunc.h"

#include <ecore/objects/base/type.h>
#include <ecore/vm/memory/gc/gc.h>


static struct Eco_TypeCore Eco_FFIFunc_TYPECORE;
       struct Eco_Type*    Eco_FFIFunc_TYPE;

void Eco_FFIFunc_Init()
{
    Eco_TypeCore_Create(&Eco_FFIFunc_TYPECORE, "Eco_FFIFunc");

    Eco_FFIFunc_TYPECORE.send = (Eco_TypeCore_SendFunc) Eco_Object_Send;
    Eco_FFIFunc_TYPECORE.mark = (Eco_TypeCore_MarkFunc) Eco_FFIFunc_Mark;
    Eco_FFIFunc_TYPECORE.del  = (Eco_TypeCore_DelFunc) Eco_FFIFunc_Del;

    Eco_FFIFunc_TYPE          = Eco_Type_NewPrefab(&Eco_FFIFunc_TYPECORE);
}

void Eco_FFIFunc_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_FFIFunc_TYPECORE);
}


struct Eco_FFIFunc* Eco_FFIFunc_New(unsigned int args, struct Eco_FFIType* rtype, struct Eco_FFIType** arg_types)
{
    struct Eco_FFIFunc*  func;
#ifdef ECO_CONFIG_USE_FFI
    unsigned int         index;
    ffi_type*            actual_args[args];
#endif

    func = Eco_Object_New(Eco_FFIFunc_TYPE, sizeof(struct Eco_FFIFunc));

    if (func != NULL)
    {
        func->arg_count = args;
#ifdef ECO_CONFIG_USE_FFI
        for (index = 0; index < args; index++)
        {
            actual_args[index] = arg_types[index]->type;
            func->arg_types[index] = arg_types[index];
        }
        func->return_type = rtype;
        ffi_prep_cif(&func->cif, FFI_DEFAULT_ABI, args, rtype->type, actual_args);
#endif
    }

    return func;
}

void Eco_FFIFunc_Mark(struct Eco_GC_State* state, struct Eco_FFIFunc* func)
{
    unsigned int  index;

#ifdef ECO_CONFIG_USE_FFI
    for (index = 0; index < func->arg_count; index++)
        Eco_GC_State_MarkObject(state, func->arg_types[index]);
    Eco_GC_State_MarkObject(state, func->return_type);
#endif
    Eco_Object_Mark(state, &func->_);
}

void Eco_FFIFunc_Del(struct Eco_FFIFunc* func)
{
    Eco_Object_Del(&func->_);
}


unsigned int Eco_FFIFunc_GetArgumentCount(struct Eco_FFIFunc* func)
{
    return func->arg_count;
}

struct Eco_FFIType* Eco_FFIFunc_GetReturnType(struct Eco_FFIFunc* func)
{
#ifdef ECO_CONFIG_USE_FFI
    return func->return_type;
#else
    return NULL;
#endif
}

struct Eco_FFIType* Eco_FFIFunc_GetArgumentType(struct Eco_FFIFunc* func, unsigned int index)
{
#ifdef ECO_CONFIG_USE_FFI
    if (index >= func->arg_count)
        return NULL;
    return func->arg_types[index];
#else
    return NULL;
#endif
}

bool Eco_FFIFunc_Call(struct Eco_FFIFunc* func, void* function, void* buffer)
{
    char*         buffer_ptr;
    unsigned int  index;
    void*         args[func->arg_count];

#ifdef ECO_CONFIG_USE_FFI
    buffer_ptr = buffer;
    for (index = 0; index < func->arg_count; index++)
    {
        args[index] = buffer_ptr;
        buffer_ptr += Eco_FFIType_SizeofCType(func->arg_types[index]);
    }
    ffi_call(&func->cif, function, buffer_ptr, args);
    return true;
#else
    return false;
#endif
}
