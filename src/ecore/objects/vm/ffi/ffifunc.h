#ifndef ECO_OBJECTS_FFI_FFIFUNC_H
#define ECO_OBJECTS_FFI_FFIFUNC_H

#include <ecore/base/config.h>

#ifdef ECO_CONFIG_USE_FFI
# include ECO_CONFIG_LIBFFI_INCLUDE
#endif

#include <ecore/objects/base/object.h>
#include <ecore/objects/vm/ffi/ffitype.h>


struct Eco_FFIFunc
{
    struct Eco_Object     _;

    unsigned int          arg_count;
#ifdef ECO_CONFIG_USE_FFI
    ffi_cif               cif;
    struct Eco_FFIType*   return_type;
    struct Eco_FFIType*   arg_types[];
#endif
};

void Eco_FFIFunc_Init();
void Eco_FFIFunc_Terminate();

struct Eco_FFIFunc* Eco_FFIFunc_New(unsigned int, struct Eco_FFIType*, struct Eco_FFIType**);
void Eco_FFIFunc_Mark(struct Eco_GC_State*, struct Eco_FFIFunc*);
void Eco_FFIFunc_Del(struct Eco_FFIFunc*);

unsigned int Eco_FFIFunc_GetArgumentCount(struct Eco_FFIFunc*);
struct Eco_FFIType* Eco_FFIFunc_GetReturnType(struct Eco_FFIFunc*);
struct Eco_FFIType* Eco_FFIFunc_GetArgumentType(struct Eco_FFIFunc*, unsigned int);

bool Eco_FFIFunc_Call(struct Eco_FFIFunc*, void*, void*);

#endif
