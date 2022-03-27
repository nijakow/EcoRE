#include "ffifunc.h"

#include <ecore/objects/base/type.h>


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
#ifdef ECO_CONFIG_USE_FFI
        for (index = 0; index < args; index++)
            actual_args[index] = arg_types[index]->type;
        ffi_prep_cif(&func->cif, FFI_DEFAULT_ABI, args, rtype->type, actual_args);
#endif
    }

    return func;
}

void Eco_FFIFunc_Mark(struct Eco_GC_State* state, struct Eco_FFIFunc* type)
{
    Eco_Object_Mark(state, &type->_);
}

void Eco_FFIFunc_Del(struct Eco_FFIFunc* type)
{
    Eco_Object_Del(&type->_);
}
