#ifndef ECO_OBJECTS_BASE_OBJECT_H
#define ECO_OBJECTS_BASE_OBJECT_H

#include <ecore/base/defs.h>

struct eco_object
{
    struct eco_type* type;
};

void* eco_object_new(struct eco_type*, eco_size_t);

static inline struct eco_type* eco_object_get_type(struct eco_object* object)
{
    return object->type;
}

#endif
