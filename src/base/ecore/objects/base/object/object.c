#include "object.h"

#include <ecore/objects/base/type.h>
#include <ecore/vm/memory/memory.h>


/*
 *    B o o k k e e p i n g   a n d   T y p e
 */

struct Eco_Arena Eco_OBJECTS;

static struct Eco_TypeCore Eco_Object_TYPECORE;
static struct Eco_Type*    Eco_Object_TYPE;

void Eco_Object_Init()
{
    Eco_Arena_Create(&Eco_OBJECTS);

    Eco_TypeCore_Create(&Eco_Object_TYPECORE, "Eco_Object");
    
    Eco_Object_TYPECORE.send          = NULL;
    Eco_Object_TYPECORE.mark_instance = Eco_Object_MarkInstance;
    Eco_Object_TYPECORE.clone         = Eco_Object_NoClone;
    Eco_Object_TYPECORE.del           = Eco_Object_Del;

    Eco_Object_TYPE           = Eco_Type_NewPrefab(&Eco_Object_TYPECORE);
}

void Eco_Object_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_Object_TYPECORE);
    Eco_Arena_Destroy(&Eco_OBJECTS);
}


/*
 *    B a s i c s
 */

void* Eco_Object_NewInArena(struct Eco_Type* type,
                            unsigned int size,
                            struct Eco_Arena* arena)
{
    struct Eco_Object* object;

    object = Eco_Memory_Alloc(size);

    object->type              = type;

    object->bits.mark_queued         = false;
    object->bits.mark_done           = false;
    object->bits.sticky              = false;
    object->bits.wants_finalization  = false;

    object->next              = arena->objects;
    arena->objects            = object;
    arena->object_count++;

    return object;
}

void* Eco_Object_New(struct Eco_Type* type,
                     unsigned int size)
{
    return Eco_Object_NewInArena(type, size, &Eco_OBJECTS);
}

struct Eco_Object* Eco_Object_NewPlain()
{
    return Eco_Object_New(Eco_Object_TYPE, sizeof(struct Eco_Object));
}


struct Eco_Object* Eco_Object_NoClone(struct Eco_CloneState* state,
                                      struct Eco_Object* original,
                                      bool forced)
{
    return original;
}

void Eco_Object_Del(struct Eco_Object* object)
{
    Eco_Memory_Free(object);
}


bool Eco_Object_IsBlessedBy(struct Eco_Object* object, struct Eco_Object* root)
{
    return false;
}
