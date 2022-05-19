#ifndef ECO_OBJECTS_FFI_FFIOBJECT_H
#define ECO_OBJECTS_FFI_FFIOBJECT_H

#include <ecore/base/config.h>
#include <ecore/objects/base/object.h>

#include "ffitype.h"

struct Eco_GC_State;

struct Eco_FFIObject
{
    struct Eco_Object    _;

    struct Eco_FFIType*    type;
    struct Eco_FFIObject*  ref;
    unsigned long          size;
    void*                  bytes;
    char                   payload[];
};

struct Eco_FFIObject* Eco_FFIObject_New(struct Eco_FFIType*, void*, unsigned long);
struct Eco_FFIObject* Eco_FFIObject_Cast(struct Eco_FFIObject*, struct Eco_FFIType*);
struct Eco_FFIObject* Eco_FFIObject_Address(struct Eco_FFIObject*);
struct Eco_FFIObject* Eco_FFIObject_Fetch(struct Eco_FFIObject*);
struct Eco_FFIObject* Eco_FFIObject_Access(struct Eco_FFIObject*, unsigned);
struct Eco_FFIObject* Eco_FFIObject_DLOpen(char*);
struct Eco_FFIObject* Eco_FFIObject_DLSym(void*, char*);
void Eco_FFIObject_Mark(struct Eco_GC_State*, struct Eco_FFIObject*);
void Eco_FFIObject_Del(struct Eco_FFIObject*);

bool Eco_FFIObject_IsFFIObject(struct Eco_Object*);
void Eco_FFIObject_AssignNull(struct Eco_FFIObject*);

static inline struct Eco_FFIType* Eco_FFIObject_GetFFIType(struct Eco_FFIObject* object)
{
    return object->type;
}

static inline unsigned long Eco_FFIObject_GetSize(struct Eco_FFIObject* object)
{
    return object->size;
}

static inline void* Eco_FFIObject_GetBytes(struct Eco_FFIObject* object)
{
    return object->bytes;
}

void Eco_FFIObject_Init();
void Eco_FFIObject_Terminate();

#endif
