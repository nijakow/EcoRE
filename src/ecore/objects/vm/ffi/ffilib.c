#include <ecore/base/config.h>

#ifdef ECO_CONFIG_USE_DLOPEN
# include <dlfcn.h>
#endif

#include <ecore/objects/base/type/type.h>
#include <ecore/objects/misc/key/key.h>
#include <ecore/vm/memory/gc/gc.h>
#include <ecore/vm/memory/memory.h>
#include <ecore/util/dwarf/dwarf.h>
#include <ecore/util/memory.h>

#include "ffilib.h"

enum Eco_FFILib_EntryType
{
    Eco_FFILib_EntryType_FFITYPE,
    Eco_FFILib_EntryType_FFIFUNC,
    Eco_FFILib_EntryType_VALUE
};

struct Eco_FFILib_Entry
{
    struct Eco_FFILib_Entry*   next;
    struct Eco_Key*            name;
    enum Eco_FFILib_EntryType  body_type;
    union {
        struct Eco_FFIType*    ffitype;
        struct Eco_FFIFunc*    ffifunc;
        Eco_Any                value;
    } body;
};


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
        object->entries = NULL;
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
    struct Eco_FFILib_Entry*  entry;

    for (entry = object->entries;
         entry != NULL;
         entry = entry->next)
    {
        Eco_GC_State_MarkObject(state, entry->name);
        switch (entry->body_type)
        {
            case Eco_FFILib_EntryType_FFITYPE:
                Eco_GC_State_MarkObject(state, entry->body.ffitype);
                break;
            case Eco_FFILib_EntryType_FFIFUNC:
                Eco_GC_State_MarkObject(state, entry->body.ffifunc);
                break;
            case Eco_FFILib_EntryType_VALUE:
                Eco_GC_State_MarkAny(state, &entry->body.value);
                break;
        }
    }
    Eco_Object_Mark(state, &object->_);
}

void Eco_FFILib_Del(struct Eco_FFILib* object)
{
    struct Eco_FFILib_Entry*  entry;

    while (object->entries != NULL)
    {
        entry           = object->entries;
        object->entries = entry->next;
        Eco_Memory_Free(entry);
    }
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
    struct Eco_FFILib_Entry*  entry;

    for (entry = lib->entries;
         entry != NULL;
         entry = entry->next)
    {
        if (entry->name == key) {
            switch (entry->body_type)
            {
                case Eco_FFILib_EntryType_FFITYPE:
                    *loc = Eco_Any_FromPointer(entry->body.ffitype);
                    break;
                case Eco_FFILib_EntryType_FFIFUNC:
                    *loc = Eco_Any_FromPointer(entry->body.ffifunc);
                    break;
                case Eco_FFILib_EntryType_VALUE:
                    *loc = entry->body.value;
                    break;
                default:
                    *loc = Eco_Any_FromPointer(lib);
            }
            return true;
        }
    }
    return false;
}

static bool Eco_FFILib_PutType(struct Eco_FFILib* lib, const char* name, struct Eco_FFIType* type)
{
    struct Eco_Key*           key;
    struct Eco_FFILib_Entry*  entry;

    key   = Eco_Key_Find(name);
    entry = Eco_Memory_Alloc(sizeof(struct Eco_FFILib_Entry));

    if (key != NULL && entry != NULL)
    {
        entry->name         = key;
        entry->body_type    = Eco_FFILib_EntryType_FFITYPE;
        entry->body.ffitype = type;
        entry->next         = lib->entries;
        lib->entries        = entry;
        return true;
    }

    return false;
}

static bool Eco_FFILib_PutValue(struct Eco_FFILib* lib, const char* name, Eco_Any value)
{
    struct Eco_Key*           key;
    struct Eco_FFILib_Entry*  entry;

    key   = Eco_Key_Find(name);
    entry = Eco_Memory_Alloc(sizeof(struct Eco_FFILib_Entry));

    if (key != NULL && entry != NULL)
    {
        entry->name       = key;
        entry->body_type  = Eco_FFILib_EntryType_VALUE;
        entry->body.value = value;
        entry->next       = lib->entries;
        lib->entries      = entry;
        return true;
    }

    return false;
}


bool Eco_FFILib_PutStruct(struct Eco_FFILib* lib, const char* name, struct Eco_FFIType* type)
{
    return Eco_FFILib_PutType(lib, name, type);
}

bool Eco_FFILib_PutUnion(struct Eco_FFILib* lib, const char* name, struct Eco_FFIType* type)
{
    return Eco_FFILib_PutType(lib, name, type);
}

bool Eco_FFILib_PutTypedef(struct Eco_FFILib* lib, const char* name, struct Eco_FFIType* type)
{
    return Eco_FFILib_PutType(lib, name, type);
}

bool Eco_FFILib_PutEnum(struct Eco_FFILib* lib, const char* name, struct Eco_FFIType* type)
{
    return Eco_FFILib_PutType(lib, name, type);
}

bool Eco_FFILib_PutEnumValue(struct Eco_FFILib* lib, const char* name, unsigned int value)
{
    return Eco_FFILib_PutValue(lib, name, Eco_Any_FromInteger((Eco_Integer) value));
}
