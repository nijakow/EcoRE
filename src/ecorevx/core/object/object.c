#include "../memory/memory.h"

#include "object.h"

void Eco_Object_Create(struct Eco_Object* self, enum Eco_ObjectType type, struct Eco_Map* map, Eco_Size_t size)
{
    self->header.gc_bits = 0x00;
    self->header.type    = type;
    self->header.size    = size;
    self->header.hash    = 0x00000000;
    self->map            = map;
    /*
     * TODO: Initialize all the slots
     */
}

struct Eco_Object* Eco_Object_AllocPlainObject(Eco_Allocator_t allocator, struct Eco_Map* map, unsigned int slot_count)
{
    const  Eco_Size_t   act_size = sizeof(struct Eco_Object) + sizeof(Eco_Any) * slot_count;
    struct Eco_Object*  self;

    self = Eco_Pages_AllocateInEden((struct Eco_Pages*) allocator, act_size);

    if (self != NULL)
    {
        Eco_Object_Create(self, Eco_ObjectType_PLAIN, map, act_size);
    }

    return self;
}
