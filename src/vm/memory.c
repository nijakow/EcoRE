#include "memory.h"

#include "../objects/object.h"


struct Eco_Object* OBJECTS = NULL;


void* Eco_Object_New(struct Eco_Type* type, unsigned int size)
{
    struct Eco_Object* object;

    object = Eco_Memory_Alloc(size);

    object->next = OBJECTS;
    object->type = type;

    return object;
}
