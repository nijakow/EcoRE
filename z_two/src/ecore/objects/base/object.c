#include <ecore/memory/memory.h>

#include "object.h"


void* eco_object_new(struct eco_type* type, eco_size_t size)
{
    struct eco_object*  object;

    object = eco_memory_alloc(size);

    if (object != NULL)
    {
        object->type = type;
    }

    return object;
}
