#ifndef ECO_OBJECTS_FFI_FFIFUNC_H
#define ECO_OBJECTS_FFI_FFIFUNC_H

#include <ecore/base/config.h>

#ifdef ECO_CONFIG_USE_FFI

#include ECO_CONFIG_LIBFFI_INCLUDE

#include <ecore/objects/base/object.h>
#include <ecore/objects/vm/ffi/ffitype.h>


struct Eco_FFIFunc
{
    struct Eco_Object     _;

    ffi_cif               cif;
};


struct Eco_FFIFunc* Eco_FFIFunc_New(unsigned int, struct Eco_FFIType*, struct Eco_FFIType**);
void Eco_FFIFunc_Mark(struct Eco_GC_State*, struct Eco_FFIFunc*);
void Eco_FFIFunc_Del(struct Eco_FFIFunc*);

#endif

void Eco_FFIFunc_Init();
void Eco_FFIFunc_Terminate();

#endif
