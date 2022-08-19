#include "ffitype.h"

#include <ecore/objects/base/type.h>
#include <ecore/objects/misc/blob/blob.h>
#include <ecore/objects/misc/string/string.h>
#include <ecore/objects/vm/ffi/ffiobject.h>
#include <ecore/vm/memory/gc/gc.h>
#include <ecore/util/memory.h>


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
    &ffi_type_pointer,
    &ffi_type_pointer
};

static Eco_Any Eco_FFIType_AsAny_Default(struct Eco_FFIType* type, void* ptr, unsigned long size)
{
    // TODO: assert(size == sizeof(Eco_FFIType_SizeofCType(type)));
    return Eco_Any_FromPointer(Eco_FFIObject_New(type, ptr, size));
}

static Eco_Any Eco_FFIType_AsAny_Void(struct Eco_FFIType* type, void* ptr, unsigned long size)
{
    return Eco_Any_FromInteger(0);
}

static Eco_Any Eco_FFIType_AsAny_Integer(struct Eco_FFIType* type, void* ptr, unsigned long size)
{
    Eco_Integer  i;

    if (size == sizeof(char)) i = *((char*) ptr);
    else if (size == sizeof(short)) i = *((short*) ptr);
    else if (size == sizeof(int)) i = *((int*) ptr);
    else if (size == sizeof(Eco_Integer)) i = *((Eco_Integer*) ptr);
    else return Eco_FFIType_AsAny_Default(type, ptr, size);
    return Eco_Any_FromInteger(i);
}

static Eco_Any Eco_FFIType_AsAny_Char(struct Eco_FFIType* type, void* ptr, unsigned long size)
{
    Eco_Codepoint  c;

    if (size == sizeof(char)) c = *((char*) ptr);
    else if (size == sizeof(short)) c = *((short*) ptr);
    else if (size == sizeof(int)) c = *((int*) ptr);
    else if (size == sizeof(Eco_Integer)) c = *((Eco_Integer*) ptr);
    else return Eco_FFIType_AsAny_Default(type, ptr, size);
    return Eco_Any_FromCharacter(c);
}

static Eco_Any Eco_FFIType_AsAny_String(struct Eco_FFIType* type, void* ptr, unsigned long size)
{
    /*
     * TODO, FIXME, XXX: This will only create a copy of the string...
     *                   what will happen to the actual data?!
     */
    return Eco_Any_FromPointer(Eco_String_New(*((char**) ptr)));
}

static bool Eco_FFIType_FromAny_Default(struct Eco_FFIType* type, void* ptr, unsigned long size, Eco_Any any)
{
    if (Eco_Any_IsPointer(any)) {
        if (Eco_FFIObject_IsFFIObject(Eco_Any_AsPointer(any)) && Eco_FFIType_SizeofCType(type) == Eco_FFIType_SizeofCType(Eco_FFIObject_GetFFIType(Eco_Any_AsPointer(any))))
            Eco_Memcpy(ptr, Eco_FFIObject_GetBytes(Eco_Any_AsPointer(any)), size);
        else if (Eco_String_IsString(Eco_Any_AsPointer(any)) && size == sizeof(char*))
            *((char**) ptr) = ((struct Eco_String*) Eco_Any_AsPointer(any))->bytes;
        else
            return false;
    } else if (Eco_Any_IsInteger(any)) {
        if (size == sizeof(char)) *((char*) ptr) = (char) Eco_Any_AsInteger(any);
        else if (size == sizeof(short)) *((short*) ptr) = (short) Eco_Any_AsInteger(any);
        else if (size == sizeof(int)) *((int*) ptr) = (int) Eco_Any_AsInteger(any);
        else if (size == sizeof(long)) *((long*) ptr) = (long) Eco_Any_AsInteger(any);
        else if (size == sizeof(long long)) *((long long*) ptr) = (long long) Eco_Any_AsInteger(any);
        else return false;
    } else if (Eco_Any_IsFloating(any)) {
        if (size == sizeof(float)) *((float*) ptr) = (float) Eco_Any_AsFloating(any);
        else if (size == sizeof(double)) *((double*) ptr) = (double) Eco_Any_AsFloating(any);
        else return false;
    } else if (Eco_Any_IsCharacter(any)) {
        return Eco_FFIType_FromAny_Default(type, ptr, size, Eco_Any_FromInteger(Eco_Any_AsCharacter(any)));
    } else {
        return false;
    }
    return true;
}


static struct Eco_TypeCore Eco_FFIType_TYPECORE;
       struct Eco_Type*    Eco_FFIType_TYPE;
static struct Eco_FFIType* Eco_FFIType_BASIC_INSTANCE_POINTERS[22];

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

    Eco_FFIType_BASIC_INSTANCE_POINTERS[0]->as_any = Eco_FFIType_AsAny_Void;
    for (index = 1; index <= 6; index++)
        Eco_FFIType_BASIC_INSTANCE_POINTERS[index]->as_any = Eco_FFIType_AsAny_Integer;
    Eco_FFIType_BASIC_INSTANCE_POINTERS[11]->as_any = Eco_FFIType_AsAny_Char;
    Eco_FFIType_BASIC_INSTANCE_POINTERS[12]->as_any = Eco_FFIType_AsAny_Char;
    for (index = 13; index <= 16; index++)
        Eco_FFIType_BASIC_INSTANCE_POINTERS[index]->as_any = Eco_FFIType_AsAny_Integer;
    Eco_FFIType_BASIC_INSTANCE_POINTERS[21]->pointee  = Eco_FFIType_BASIC_INSTANCE_POINTERS[12];    // Pointer to schar
    Eco_FFIType_BASIC_INSTANCE_POINTERS[21]->as_any   = Eco_FFIType_AsAny_String;
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
        type->from_any     = Eco_FFIType_FromAny_Default;
        type->pointer      = NULL;
        type->pointee      = NULL;
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
                                          unsigned int         member_count,
                                          bool                 is_union)
{
    struct Eco_FFIType*  type;
    char*                payload;
    unsigned int         index;

#ifdef ECO_CONFIG_USE_FFI
    type = Eco_Object_New(Eco_FFIType_TYPE, sizeof(struct Eco_FFIType) + sizeof(struct Eco_FFIType*) * member_count + sizeof(struct Eco_FFIType_Entry) * (member_count + 1));
#else
    type = Eco_Object_New(Eco_FFIType_TYPE, sizeof(struct Eco_FFIType) + sizeof(struct Eco_FFIType*) * member_count);
#endif

    if (type != NULL)
    {
        payload = ((char*) type) + sizeof(struct Eco_FFIType) + sizeof(struct Eco_FFIType*) * member_count;
        type->member_count = member_count;
        type->as_any       = Eco_FFIType_AsAny_Default;
        type->from_any     = Eco_FFIType_FromAny_Default;
        type->pointer      = NULL;
        type->pointee      = NULL;
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
        if (is_union) {
            type->type->size = 0;
            for (index = 0; index < member_count; index++)
            {
                const unsigned int s = Eco_FFIType_SizeofCType(members[index]);
                offsets[index] = 0;
                if (type->type->size < s)
                    type->type->size = s;
            }
        } else {
            ffi_get_struct_offsets(FFI_DEFAULT_ABI, type->type, offsets);
        }
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

struct Eco_FFIType* Eco_FFIType_NewArray(struct Eco_FFIType* type, unsigned int size)
{
    unsigned int         index;
    struct Eco_FFIType*  elements[size];

    for (index = 0; index < size; index++)
        elements[index] = type;
    
    return Eco_FFIType_NewStruct(elements, NULL, size, false);
}

struct Eco_FFIType* Eco_FFIType_PointerTo(struct Eco_FFIType* pointee)
{
    struct Eco_FFIType*  type;

    if (pointee->pointer != NULL)
        return pointee->pointer;

    type = Eco_FFIType_New(&ffi_type_pointer);

    if (type != NULL)
    {
        pointee->pointer = type;
        type->pointee    = pointee;
    }

    return type;
}

void Eco_FFIType_Mark(struct Eco_GC_State* state, struct Eco_FFIType* type)
{
    unsigned int  index;

    if (type->pointee != NULL)
        Eco_GC_State_MarkObject(state, type->pointee);
    if (type->pointer != NULL)
        Eco_GC_State_MarkObject(state, type->pointer);
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

struct Eco_FFIType* Eco_FFIType_GetVoidPointer()
{
    return Eco_FFIType_PointerTo(Eco_FFIType_GetForIndex(0));
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
