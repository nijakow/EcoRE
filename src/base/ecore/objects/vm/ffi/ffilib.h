#ifndef ECO_OBJECTS_FFI_FFILIB_H
#define ECO_OBJECTS_FFI_FFILIB_H

#include <ecore/base/config.h>
#include <ecore/objects/base/object.h>
#include <ecore/objects/misc/key/key.h>

#include "ffitype.h"
#include "ffifunc.h"

struct Eco_GC_State;

struct Eco_FFILib_Entry;

struct Eco_FFILib
{
    struct Eco_Object         _;
    
    void*                     dl_handle;

    struct Eco_FFILib_Entry*  entries;
};

struct Eco_FFILib* Eco_FFILib_New(const char*);
void Eco_FFILib_MarkChildren(struct Eco_GC_State*, struct Eco_FFILib*);
void Eco_FFILib_MarkInstance(struct Eco_GC_State*, struct Eco_FFILib*);
void Eco_FFILib_Del(struct Eco_FFILib*);

bool Eco_FFILib_At(struct Eco_FFILib*, struct Eco_Key*, Eco_Any*);

bool Eco_FFILib_PutStruct(struct Eco_FFILib*, const char*, struct Eco_FFIType*);
bool Eco_FFILib_PutUnion(struct Eco_FFILib*, const char*, struct Eco_FFIType*);
bool Eco_FFILib_PutTypedef(struct Eco_FFILib*, const char*, struct Eco_FFIType*);
bool Eco_FFILib_PutEnum(struct Eco_FFILib*, const char*, struct Eco_FFIType*);
bool Eco_FFILib_PutEnumValue(struct Eco_FFILib*, const char*, unsigned int);
bool Eco_FFILib_PutFunction(struct Eco_FFILib*, const char*, struct Eco_FFIFunc*);

void Eco_FFILib_Init();
void Eco_FFILib_Terminate();

#endif
