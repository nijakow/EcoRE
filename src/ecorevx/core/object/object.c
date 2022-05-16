#include "../memory/memory.h"

#include "object.h"

static void Eco_Object_Create(struct Eco_Object* self, enum Eco_ObjectType type, Eco_Size_t size)
{
    self->header.gc_bits = 0x00;
    self->header.type    = type;
    self->header.size    = size;
    self->header.hash    = 0x00000000;
    /*
     * TODO: Initialize all the slots
     */
}

struct Eco_Object* Eco_Object_AllocPlainObject(Eco_Allocator_t allocator, Eco_Any type, unsigned int slot_count)
{
    const  Eco_Size_t   act_size = sizeof(struct Eco_Object) + sizeof(Eco_Any) * slot_count;
    struct Eco_Object*  self;

    self = Eco_Pages_AllocateInEden((struct Eco_Pages*) allocator, act_size);

    if (self != NULL)
    {
        Eco_Object_Create(self, Eco_ObjectType_PLAIN, act_size);
        Eco_Object_Set(self, 0, type);
    }

    return self;
}

struct Eco_Object* Eco_Object_AllocByteObject(Eco_Allocator_t allocator, Eco_Any type, Eco_Size_t size)
{
    const  Eco_Size_t   act_size = sizeof(struct Eco_Object) + sizeof(Eco_Any) * Eco_DEFAULT_OBJECT_SLOT_COUNT + size;
    struct Eco_Object*  self;

    self = Eco_Pages_AllocateInEden((struct Eco_Pages*) allocator, act_size);

    if (self != NULL)
    {
        Eco_Object_Create(self, Eco_ObjectType_BYTES, act_size);
        Eco_Object_Set(self, 0, type);
    }

    return self;
}
