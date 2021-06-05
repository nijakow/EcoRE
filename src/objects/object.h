#pragma once

#include "../eco.h"


/*
 *
 *    s t r u c t   E c o _ O b j e c t
 *
 */

struct Eco_Object
{
    struct Eco_Object* next;
    struct Eco_Type*   type;
};

static inline void* Eco_Object_At(struct Eco_Object* object, unsigned int offset)
{
    return (void*) (((char*) object) + sizeof(struct Eco_Object) + offset);
}
