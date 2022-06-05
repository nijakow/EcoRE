#include <ecore/base/config.h>

#ifdef ECO_CONFIG_USE_DLOPEN
# include <dlfcn.h>
#endif

#include <ecore/objects/base/type/type.h>
#include <ecore/vm/memory/gc/gc.h>
#include <ecore/util/dwarf/dwarf.h>
#include <ecore/util/memory.h>

#include "ffilib.h"

static struct Eco_TypeCore Eco_FFILib_TYPECORE;
       struct Eco_Type*    Eco_FFILib_TYPE;

void Eco_FFILib_Init()
{
    Eco_TypeCore_Create(&Eco_FFILib_TYPECORE, "Eco_FFILib");

    Eco_FFILib_TYPECORE.send = (Eco_TypeCore_SendFunc) Eco_Object_Send;
    Eco_FFILib_TYPECORE.mark = (Eco_TypeCore_MarkFunc) Eco_FFILib_Mark;
    Eco_FFILib_TYPECORE.del  = (Eco_TypeCore_DelFunc) Eco_FFILib_Del;

    Eco_FFILib_TYPE          = Eco_Type_NewPrefab(&Eco_FFILib_TYPECORE);
}

void Eco_FFILib_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_FFILib_TYPECORE);
}


struct Eco_FFILib* Eco_FFILib_New(const char* path)
{
    struct Eco_FFILib*  object;

    object = Eco_Object_New(Eco_FFILib_TYPE, sizeof(struct Eco_FFILib));

    if (object != NULL)
    {
#ifndef ECO_CONFIG_USE_DLOPEN
        object->dl_handle = dlopen(path, RTLD_NOW);
#else
        object->dl_handle = NULL;
#endif
        Eco_Dwarf_LoadDebugInfo(path, object);
    }

    return object;
}

void Eco_FFILib_Mark(struct Eco_GC_State* state, struct Eco_FFILib* object)
{
    Eco_Object_Mark(state, &object->_);
}

void Eco_FFILib_Del(struct Eco_FFILib* object)
{
    if (object->dl_handle != NULL)
    {
#ifdef ECO_CONFIG_USE_DLOPEN
        dlclose(object->dl_handle);
#endif
        object->dl_handle = NULL;
    }
    Eco_Object_Del(&object->_);
}


bool Eco_FFILib_At(struct Eco_FFILib* lib, struct Eco_Key* key, Eco_Any* loc)
{
    /* TODO */
    return false;
}

void Eco_FFILib_Put(struct Eco_FFILib* lib, struct Eco_Key* key, Eco_Any value)
{
    /* TODO */
}

void Eco_FFILib_PutType(struct Eco_FFILib* lib, struct Eco_Key* key, struct Eco_FFIType* value)
{
    Eco_FFILib_Put(lib, key, Eco_Any_FromPointer(value));
}
