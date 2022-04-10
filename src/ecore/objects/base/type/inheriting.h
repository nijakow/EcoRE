#ifndef ECO_OBJECTS_BASE_TYPE_INHERITING_H
#define ECO_OBJECTS_BASE_TYPE_INHERITING_H

#include "type.h"

struct Eco_TypeLink
{
    struct Eco_Type*      super;
    struct Eco_Type*      sub;
    struct Eco_TypeLink*  right;
    struct Eco_TypeLink*  down;
    unsigned int          slot_index;
};

void Eco_Type_EstablishTypeLink(struct Eco_Type*, unsigned int, struct Eco_Type*);
void Eco_Type_RemoveAllTypeLinks(struct Eco_Type*);

#endif
