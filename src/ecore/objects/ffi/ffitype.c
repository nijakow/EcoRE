#include "ffitype.h"

#include <ecore/objects/base/type.h>


static struct Eco_TypeCore Eco_FFIType_TYPECORE;
       struct Eco_Type*    Eco_FFIType_TYPE;

void Eco_FFIType_Init()
{
    Eco_TypeCore_Create(&Eco_FFIType_TYPECORE, "Eco_FFIType");

    Eco_FFIType_TYPECORE.send = (Eco_TypeCore_SendFunc) Eco_Object_Send;
    Eco_FFIType_TYPECORE.mark = (Eco_TypeCore_MarkFunc) Eco_FFIType_Mark;
    Eco_FFIType_TYPECORE.del  = (Eco_TypeCore_DelFunc) Eco_FFIType_Del;

    Eco_FFIType_TYPE          = Eco_Type_NewPrefab(&Eco_FFIType_TYPECORE);
}

void Eco_FFIType_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_FFIType_TYPECORE);
}


struct Eco_FFIType* Eco_FFIType_New(ffi_type* ffi_type_ptr)
{
    struct Eco_FFIType*  type;

    type = Eco_Object_New(Eco_FFIType_TYPE, sizeof(struct Eco_FFIType));

    if (type != NULL)
    {
        if (ffi_type_ptr == NULL)
            type->type = &type->payload;    // TODO: Initialize payload
        else
            type->type = ffi_type_ptr;
    }

    return type;
}

void Eco_FFIType_Mark(struct Eco_GC_State* state, struct Eco_FFIType* type)
{
    Eco_Object_Mark(state, &type->_);
}

void Eco_FFIType_Del(struct Eco_FFIType* type)
{
    Eco_Object_Del(&type->_);
}


/*
 *    G e t F o r I n d e x
 */

static ffi_type* Eco_FFIType_BASIC_TYPE_POINTERS[] = {
    &ffi_type_void,
    &ffi_type_uint8,
    &ffi_type_sint8,
    &ffi_type_uint16,
    &ffi_type_sint16,
    &ffi_type_uint32,
    &ffi_type_sint32,
    &ffi_type_uint64,
    &ffi_type_sint64,
    &ffi_type_float,
    &ffi_type_double,
    &ffi_type_uchar,
    &ffi_type_schar,
    &ffi_type_ushort,
    &ffi_type_sshort,
    &ffi_type_uint,
    &ffi_type_sint,
    &ffi_type_ulong,
    &ffi_type_slong,
    &ffi_type_longdouble,
    &ffi_type_pointer
};

struct Eco_FFIType* Eco_FFIType_GetForIndex(unsigned int index)
{
    // TODO: Cache these instances
    return Eco_FFIType_New(Eco_FFIType_BASIC_TYPE_POINTERS[index]);
}
