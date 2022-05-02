#ifndef ECO_OBJECTS_FFI_FFITYPE_H
#define ECO_OBJECTS_FFI_FFITYPE_H

#include <ecore/base/config.h>

#ifdef ECO_CONFIG_USE_FFI
# include ECO_CONFIG_LIBFFI_INCLUDE
#endif

#include <ecore/objects/base/object.h>
#include <ecore/objects/misc/key/key.h>


struct Eco_FFIType_Entry
{
    struct Eco_Key*      name;
    struct Eco_FFIType*  type;
    unsigned int         offset;
};

struct Eco_FFIType
{
    struct Eco_Object         _;

#ifdef ECO_CONFIG_USE_FFI
    ffi_type*                 type;
    ffi_type                  payload;
#endif
    struct Eco_FFIType*       pointer;
    struct Eco_FFIType*       pointee;
    Eco_Any                 (*as_any)(struct Eco_FFIType*, void*, unsigned long);
    bool                    (*from_any)(struct Eco_FFIType*, void*, unsigned long, Eco_Any);
    unsigned int              member_count;
    struct Eco_FFIType_Entry  members[0];
};

#ifdef ECO_CONFIG_USE_FFI
struct Eco_FFIType* Eco_FFIType_New(ffi_type*);
#else
struct Eco_FFIType* Eco_FFIType_New(void*);
#endif
struct Eco_FFIType* Eco_FFIType_NewStruct(struct Eco_FFIType**, struct Eco_Key**, unsigned int);
struct Eco_FFIType* Eco_FFIType_PointerTo(struct Eco_FFIType*);
void Eco_FFIType_Mark(struct Eco_GC_State*, struct Eco_FFIType*);
void Eco_FFIType_Del(struct Eco_FFIType*);

#ifdef ECO_CONFIG_USE_FFI

struct Eco_FFIType* Eco_FFIType_GetForIndex(unsigned int);
struct Eco_FFIType* Eco_FFIType_GetVoidPointer();

static inline struct Eco_FFIType* Eco_FFIType_PointeeOrSelf(struct Eco_FFIType* type)
{
    return (type->pointee == NULL) ? type : type->pointee;
}

static inline unsigned int Eco_FFIType_SizeofCType(struct Eco_FFIType* type) {
    return type->type->size;
}

unsigned int Eco_FFIType_OffsetOf_ByIndex(struct Eco_FFIType*, unsigned int);
unsigned int Eco_FFIType_OffsetOf_ByName(struct Eco_FFIType*, struct Eco_Key*);

static inline struct Eco_FFIType* Eco_FFIType_TypeOf_ByIndex(struct Eco_FFIType* type, unsigned int index)
{
    if (index >= type->member_count)
        return NULL;
    return type->members[index].type;
}

static inline struct Eco_Key* Eco_FFIType_NameOf_ByIndex(struct Eco_FFIType* type, unsigned int index)
{
    if (index >= type->member_count)
        return NULL;
    return type->members[index].name;
}


static inline unsigned int Eco_FFIType_GetStructMemberCount(struct Eco_FFIType* type)
{
    return type->member_count;
}

#endif

void Eco_FFIType_Init();
void Eco_FFIType_Terminate();

#endif
