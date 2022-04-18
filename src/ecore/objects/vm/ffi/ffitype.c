#include "ffitype.h"

#include <ecore/objects/base/type.h>
#include <ecore/objects/misc/blob/blob.h>
#include <ecore/vm/memory/gc/gc.h>

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

static Eco_Any Eco_FFIType_AsAny_Default(void* ptr, unsigned long size)
{
    return Eco_Any_FromPointer(Eco_Blob_NewFromData(ptr, size));
}

static struct Eco_TypeCore Eco_FFIType_TYPECORE;
       struct Eco_Type*    Eco_FFIType_TYPE;
static struct Eco_FFIType* Eco_FFIType_BASIC_INSTANCE_POINTERS[21];

void Eco_FFIType_Init()
{
    unsigned int  index;

    Eco_TypeCore_Create(&Eco_FFIType_TYPECORE, "Eco_FFIType");

    Eco_FFIType_TYPECORE.send = (Eco_TypeCore_SendFunc) Eco_Object_Send;
    Eco_FFIType_TYPECORE.mark = (Eco_TypeCore_MarkFunc) Eco_FFIType_Mark;
    Eco_FFIType_TYPECORE.del  = (Eco_TypeCore_DelFunc) Eco_FFIType_Del;

    Eco_FFIType_TYPE          = Eco_Type_NewPrefab(&Eco_FFIType_TYPECORE);

    for (index = 0; index < sizeof(Eco_FFIType_BASIC_TYPE_POINTERS) / sizeof(Eco_FFIType_BASIC_TYPE_POINTERS[0]); index++)
    {
        Eco_FFIType_BASIC_INSTANCE_POINTERS[index] = Eco_FFIType_New(Eco_FFIType_BASIC_TYPE_POINTERS[index]);
        Eco_Object_MakeSticky(&Eco_FFIType_BASIC_INSTANCE_POINTERS[index]->_);
    }
}

void Eco_FFIType_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_FFIType_TYPECORE);
}

#ifdef ECO_CONFIG_USE_FFI
struct Eco_FFIType* Eco_FFIType_New(ffi_type* ffi_type_ptr)
#else
struct Eco_FFIType* Eco_FFIType_New(void* ffi_type_ptr)
#endif
{
    struct Eco_FFIType*  type;

    type = Eco_Object_New(Eco_FFIType_TYPE, sizeof(struct Eco_FFIType));

    if (type != NULL)
    {
        type->member_count = 0;
        type->as_any       = Eco_FFIType_AsAny_Default;
#ifdef ECO_CONFIG_USE_FFI
        if (ffi_type_ptr == NULL)
            type->type = &type->payload;    // TODO: Initialize payload
        else
            type->type = ffi_type_ptr;
#endif
    }

    return type;
}

struct Eco_FFIType* Eco_FFIType_NewStruct(struct Eco_FFIType** members,
                                          struct Eco_Key**     names,
                                          unsigned int         member_count)
{
    struct Eco_FFIType*  type;
    char*                payload;
    unsigned int         index;

#ifdef ECO_CONFIG_USE_FFI
    type = Eco_Object_New(Eco_FFIType_TYPE, sizeof(struct Eco_FFIType) + sizeof(struct Eco_FFIType*) * member_count + sizeof(ffi_type*) * (member_count + 1));
#else
    type = Eco_Object_New(Eco_FFIType_TYPE, sizeof(struct Eco_FFIType) + sizeof(struct Eco_FFIType*) * member_count);
#endif

    if (type != NULL)
    {
        payload = ((char*) type) + sizeof(struct Eco_FFIType) + sizeof(struct Eco_FFIType*) * member_count;
        type->member_count = member_count;
        type->as_any       = Eco_FFIType_AsAny_Default;
        size_t offsets[member_count];
#ifdef ECO_CONFIG_USE_FFI
        for (index = 0; index < member_count; index++)
            ((ffi_type**) payload)[index] = members[index]->type;
        ((ffi_type**) payload)[index] = NULL;
        type->payload.size      = 0; /* LibFFI initializes this */
        type->payload.alignment = 0; /* LibFFI initializes this */
        type->payload.type      = FFI_TYPE_STRUCT;
        type->payload.elements  = (ffi_type**) payload;
        type->type = &type->payload;
        ffi_get_struct_offsets(FFI_DEFAULT_ABI, &type->payload, offsets);
#endif
        for (index = 0; index < member_count; index++)
        {
            type->members[index].name   = (names == NULL) ? NULL : names[index];
            type->members[index].type   = members[index];
            type->members[index].offset = offsets[index];
        }
    }

    return type;
}

void Eco_FFIType_Mark(struct Eco_GC_State* state, struct Eco_FFIType* type)
{
    unsigned int  index;

    for (index = 0; index < type->member_count; index++)
    {
        if (type->members[index].name != NULL)
            Eco_GC_State_MarkObject(state, type->members[index].name);
        Eco_GC_State_MarkObject(state, type->members[index].type);
    }
    Eco_Object_Mark(state, &type->_);
}

void Eco_FFIType_Del(struct Eco_FFIType* type)
{
    Eco_Object_Del(&type->_);
}


#ifdef ECO_CONFIG_USE_FFI

/*
 *    G e t F o r I n d e x
 */

struct Eco_FFIType* Eco_FFIType_GetForIndex(unsigned int index)
{
    return Eco_FFIType_BASIC_INSTANCE_POINTERS[index];
}

unsigned int Eco_FFIType_OffsetOf_ByIndex(struct Eco_FFIType* type, unsigned int index)
{
    if (index >= type->member_count)
        return 0;
    return type->members[index].offset;
}

unsigned int Eco_FFIType_OffsetOf_ByName(struct Eco_FFIType* type, struct Eco_Key* name)
{
    unsigned int  index;

    for (index = 0; index < type->member_count; index++)
    {
        if (type->members[index].name == name)
            return type->members[index].offset;
    }
    return 0;
}

#endif
