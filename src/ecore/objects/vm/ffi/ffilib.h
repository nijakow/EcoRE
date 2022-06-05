#ifndef ECO_OBJECTS_FFI_FFILIB_H
#define ECO_OBJECTS_FFI_FFILIB_H

#include <ecore/base/config.h>
#include <ecore/objects/base/object.h>
#include <ecore/objects/misc/key/key.h>

#include "ffitype.h"

struct Eco_GC_State;

struct Eco_FFILib
{
    struct Eco_Object  _;
    
    void*              dl_handle;
};

struct Eco_FFILib* Eco_FFILib_New();
void Eco_FFILib_Mark(struct Eco_GC_State*, struct Eco_FFILib*);
void Eco_FFILib_Del(struct Eco_FFILib*);

bool Eco_FFILib_At(struct Eco_FFILib*, struct Eco_Key*, Eco_Any*);
void Eco_FFILib_Put(struct Eco_FFILib*, struct Eco_Key*, Eco_Any);
void Eco_FFILib_PutType(struct Eco_FFILib*, struct Eco_Key*, struct Eco_FFIType*);

void Eco_FFILib_Init();
void Eco_FFILib_Terminate();

#endif