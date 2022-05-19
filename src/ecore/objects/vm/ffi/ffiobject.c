#include <ecore/base/config.h>

#ifdef ECO_CONFIG_USE_DLOPEN
# include <dlfcn.h>
#endif

#include <ecore/objects/base/type/type.h>
#include <ecore/vm/memory/gc/gc.h>
#include <ecore/util/memory.h>

#include "ffiobject.h"


static struct Eco_TypeCore Eco_FFIObject_TYPECORE;
       struct Eco_Type*    Eco_FFIObject_TYPE;

void Eco_FFIObject_Init()
{
    Eco_TypeCore_Create(&Eco_FFIObject_TYPECORE, "Eco_FFIObject");

    Eco_FFIObject_TYPECORE.send = (Eco_TypeCore_SendFunc) Eco_Object_Send;
    Eco_FFIObject_TYPECORE.mark = (Eco_TypeCore_MarkFunc) Eco_FFIObject_Mark;
    Eco_FFIObject_TYPECORE.del  = (Eco_TypeCore_DelFunc) Eco_FFIObject_Del;

    Eco_FFIObject_TYPE          = Eco_Type_NewPrefab(&Eco_FFIObject_TYPECORE);
}

void Eco_FFIObject_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_FFIObject_TYPECORE);
}


struct Eco_FFIObject* Eco_FFIObject_New(struct Eco_FFIType* type, void* ptr, unsigned long size)
{
    struct Eco_FFIObject*  object;
    unsigned long          index;

    object = Eco_Object_New(Eco_FFIObject_TYPE, sizeof(struct Eco_FFIObject) + size);

    if (object != NULL)
    {
        object->type  = type;
        object->size  = size;
        object->bytes = object->payload;
        object->ref   = NULL;
        if (ptr == NULL) {
            for (index = 0; index < size; index++)
                ((char*) object->bytes)[index] = 0;
        } else {
            Eco_Memcpy(object->bytes, ptr, size);
        }
    }

    return object;
}

struct Eco_FFIObject* Eco_FFIObject_Cast(struct Eco_FFIObject* original, struct Eco_FFIType* type)
{
    struct Eco_FFIObject*  object;

    object = Eco_Object_New(Eco_FFIObject_TYPE, sizeof(struct Eco_FFIObject));

    if (object != NULL)
    {
        object->type  = type;
        object->size  = original->size;
        object->bytes = original->bytes;
        object->ref   = original;
    }

    return object;
}

struct Eco_FFIObject* Eco_FFIObject_Address(struct Eco_FFIObject* original)
{
    struct Eco_FFIObject*  object;

    object = Eco_Object_New(Eco_FFIObject_TYPE, sizeof(struct Eco_FFIObject) + sizeof(void*));

    if (object != NULL)
    {
        object->type                = Eco_FFIType_PointerTo(Eco_FFIObject_GetFFIType(original));
        object->size                = sizeof(void*);
        object->bytes               = object->payload;
        object->ref                 = original;
        *((void**) object->payload) = Eco_FFIObject_GetBytes(original);
    }

    return object;
}

Eco_Any Eco_FFIObject_Fetch(struct Eco_FFIObject* object)
{
    struct Eco_FFIType*    deref_type;

    deref_type = Eco_FFIType_PointeeOrSelf(Eco_FFIObject_GetFFIType(object));
    return deref_type->as_any(deref_type, *((void**) object->bytes), Eco_FFIType_SizeofCType(deref_type));
}

struct Eco_FFIObject* Eco_FFIObject_Access(struct Eco_FFIObject* original, unsigned int index)
{
    struct Eco_FFIObject*  object;

    // TODO: Index check!

    object = Eco_Object_New(Eco_FFIObject_TYPE, sizeof(struct Eco_FFIObject) + sizeof(void*));

    if (object != NULL)
    {
        object->type                = Eco_FFIType_PointerTo(original->type->members[index].type);
        object->size                = sizeof(void*);
        object->bytes               = object->payload;
        object->ref                 = original;
        *((void**) object->payload) = Eco_FFIObject_GetBytes(original) + original->type->members[index].offset;
    }

    return object;
}

struct Eco_FFIObject* Eco_FFIObject_DLOpen(char* path)
{
#ifdef ECO_CONFIG_USE_DLOPEN
    void* ptr;

    ptr = dlopen(path, RTLD_NOW);
    if (ptr == NULL)
        return NULL;
    return Eco_FFIObject_New(Eco_FFIType_GetVoidPointer(), &ptr, sizeof(ptr));
#else
    return NULL;
#endif
}

struct Eco_FFIObject* Eco_FFIObject_DLSym(void* base, char* symbol)
{
#ifdef ECO_CONFIG_USE_DLOPEN
    void* symptr;

#ifdef RTLD_DEFAULT
    if (base == NULL)
        base = RTLD_DEFAULT;
#endif

    symptr = dlsym(base, symbol);
    if (symptr == NULL)
        return NULL;
    return Eco_FFIObject_New(Eco_FFIType_GetVoidPointer(), &symptr, sizeof(symptr));
#else
    return NULL;
#endif
}

void Eco_FFIObject_Mark(struct Eco_GC_State* state, struct Eco_FFIObject* object)
{
    if (object->ref != NULL)
        Eco_GC_State_MarkObject(state, object->ref);
    Eco_GC_State_MarkObject(state, object->type);
    Eco_Object_Mark(state, &object->_);
}

void Eco_FFIObject_Del(struct Eco_FFIObject* object)
{
    Eco_Object_Del(&object->_);
}

bool Eco_FFIObject_IsFFIObject(struct Eco_Object* object)
{
    return object->type->typecore == &Eco_FFIObject_TYPECORE;
}

void Eco_FFIObject_AssignNull(struct Eco_FFIObject* object)
{
    // TODO: Insert a type check if the type of object is 'pointer'!
    if (object->bytes != NULL)
        *((void**) (object->bytes)) = NULL;
}
