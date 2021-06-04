#pragma once

#include "../eco.h"


/*
 *
 *    s t r u c t   E c o _ O b j e c t
 *
 */

struct Eco_Object
{
    char payload[];
};

static inline void* Eco_Object_At(struct Eco_Object* object, unsigned int offset)
{
    return (void*) &(object->payload[offset]);
}



/*
 *
 *    s t r u c t   E c o _ S h i p
 *
 */

struct Eco_Ship
{
    struct Eco_Ship *next;
    struct Eco_Type *type;
    struct Eco_Object payload;
};

