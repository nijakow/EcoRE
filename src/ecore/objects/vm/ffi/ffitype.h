#ifndef ECO_OBJECTS_FFI_FFITYPE_H
#define ECO_OBJECTS_FFI_FFITYPE_H

#include <ecore/base/config.h>

#ifdef ECO_CONFIG_USE_FFI

#include ECO_CONFIG_LIBFFI_INCLUDE

#include <ecore/objects/base/object.h>


struct Eco_FFIType
{
    struct Eco_Object     _;

    ffi_type*             type;
    ffi_type              payload;
};


struct Eco_FFIType* Eco_FFIType_New(ffi_type*);
void Eco_FFIType_Mark(struct Eco_GC_State*, struct Eco_FFIType*);
void Eco_FFIType_Del(struct Eco_FFIType*);

struct Eco_FFIType* Eco_FFIType_GetForIndex(unsigned int);

#endif

void Eco_FFIType_Init();
void Eco_FFIType_Terminate();

#endif
