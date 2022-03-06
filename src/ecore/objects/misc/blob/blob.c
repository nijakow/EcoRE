#include "blob.h"

#include <ecore/objects/base/type.h>
#include <ecore/objects/misc/string/string.h>
#include <ecore/vm/memory/memory.h>
#include <ecore/vm/memory/gc/gc.h>
#include <ecore/vm/core/clone.h>
#include <ecore/vm/core/send.h>
#include <ecore/util/memory.h>


/*
 *    T y p e
 */

static struct Eco_TypeCore Eco_Blob_TYPECORE;
       struct Eco_Type*    Eco_Blob_TYPE;


void Eco_Blob_Init()
{
    Eco_TypeCore_Create(&Eco_Blob_TYPECORE, "Eco_Blob");

    Eco_Blob_TYPECORE.send  = (Eco_TypeCore_SendFunc)  Eco_Object_Send;
    Eco_Blob_TYPECORE.mark  = (Eco_TypeCore_MarkFunc)  Eco_Blob_Mark;
    Eco_Blob_TYPECORE.clone = (Eco_TypeCore_CloneFunc) Eco_Blob_Clone;
    Eco_Blob_TYPECORE.del   = (Eco_TypeCore_DelFunc)   Eco_Blob_Del;

    Eco_Blob_TYPE           = Eco_Type_NewPrefab(&Eco_Blob_TYPECORE);
}

void Eco_Blob_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_Blob_TYPECORE);
}


/*
 *    B a s i c s
 */

struct Eco_Blob* Eco_Blob_New(unsigned int element_count)
{
    struct Eco_Blob*  blob;

    blob = Eco_Object_New(Eco_Blob_TYPE, sizeof(struct Eco_Blob) + sizeof(char) * element_count);

    if (blob != NULL)
    {
        blob->size = element_count;
    }

    return blob;
}

void Eco_Blob_Mark(struct Eco_GC_State* state, struct Eco_Blob* blob)
{
    Eco_Object_Mark(state, &blob->_);
}

struct Eco_Blob* Eco_Blob_Clone(struct Eco_CloneState* state,
                                struct Eco_Blob* blob,
                                bool forced)
{
    struct Eco_Blob*  the_clone;

    if (!forced) return blob;

    the_clone = Eco_Blob_New(Eco_Blob_Size(blob));
    
    Eco_CloneState_RegisterClone(state,
                                 (struct Eco_Object*) blob,
                                 (struct Eco_Object*) the_clone);

    Eco_Memcpy(the_clone->bytes, blob->bytes, Eco_Blob_Size(the_clone));

    return the_clone;
}

void Eco_Blob_Del(struct Eco_Blob* blob)
{
    Eco_Object_Del(&blob->_);
}
