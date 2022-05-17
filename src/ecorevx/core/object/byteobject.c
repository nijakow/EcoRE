#include "byteobject.h"

struct Eco_Object* Eco_Object_AllocByteObject(Eco_Allocator_t allocator, struct Eco_Map* map, Eco_Size_t size)
{
    const  Eco_Size_t   act_size = sizeof(struct Eco_Object) + sizeof(Eco_Any) * Eco_DEFAULT_OBJECT_SLOT_COUNT + size;
    struct Eco_Object*  self;

    self = Eco_Pages_AllocateInEden((struct Eco_Pages*) allocator, act_size);

    if (self != NULL)
    {
        Eco_Object_Create(self, Eco_ObjectType_BYTES, map, act_size);
    }

    return self;
}
