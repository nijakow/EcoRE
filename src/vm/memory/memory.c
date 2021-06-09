#include "memory.h"

#include "../../objects/object.h"
#include "../../objects/type.h"


struct Eco_Object* Eco_OBJECTS = NULL;


void* Eco_Object_New(struct Eco_Type* type, unsigned int size)
{
    struct Eco_Object* object;

    object = Eco_Memory_Alloc(size);

    object->next = Eco_OBJECTS;
    Eco_OBJECTS = object;

    object->type = type;

    return object;
}
