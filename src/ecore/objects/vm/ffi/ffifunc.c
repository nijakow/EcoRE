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


struct Eco_FFIFunc* Eco_FFIFunc_New(unsigned int args, ffi_type* rtype, ffi_type** arg_types)
{
    struct Eco_FFIFunc*  func;

    func = Eco_Object_New(Eco_FFIFunc_TYPE, sizeof(struct Eco_FFIFunc));

    if (func != NULL)
    {
        ffi_prep_cif(&func->cif, FFI_DEFAULT_ABI, args, rtype, arg_types);
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
