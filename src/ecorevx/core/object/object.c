#include "object.h"

void Eco_Object_Create(struct Eco_Object* self, enum Eco_ObjectType type, Eco_Size_t size)
{
    self->header.gc_bits = 0x00;
    self->header.type    = type;
    self->header.size    = size;
    self->header.hash    = 0x00000000;
    /*
     * TODO: Initialize all the slots
     */
}

void Eco_Object_Destroy(struct Eco_Object* self)
{
}
