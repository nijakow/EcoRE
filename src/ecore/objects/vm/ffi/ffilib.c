#include <ecore/base/config.h>

#include <ecore/objects/base/type/type.h>
#include <ecore/vm/memory/gc/gc.h>
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


struct Eco_FFILib* Eco_FFILib_New()
{
    struct Eco_FFILib*  object;

    object = Eco_Object_New(Eco_FFILib_TYPE, sizeof(struct Eco_FFILib));

    if (object != NULL)
    {
    }

    return object;
}

void Eco_FFILib_Mark(struct Eco_GC_State* state, struct Eco_FFILib* object)
{
    Eco_Object_Mark(state, &object->_);
}

void Eco_FFILib_Del(struct Eco_FFILib* object)
{
    Eco_Object_Del(&object->_);
}
