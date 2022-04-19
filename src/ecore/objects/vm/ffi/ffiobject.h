#ifndef ECO_OBJECTS_FFI_FFIOBJECT_H
#define ECO_OBJECTS_FFI_FFIOBJECT_H

#include <ecore/base/config.h>
#include <ecore/objects/base/object.h>

#include "ffitype.h"

struct Eco_GC_State;

struct Eco_FFIObject
{
    struct Eco_Object    _;

    struct Eco_FFIType*  type;
    unsigned long        size;
    void*                bytes;
    char                 payload[];
};

struct Eco_FFIObject* Eco_FFIObject_New(struct Eco_FFIType*, void*, unsigned long);
void Eco_FFIObject_Mark(struct Eco_GC_State*, struct Eco_FFIObject*);
void Eco_FFIObject_Del(struct Eco_FFIObject*);

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
