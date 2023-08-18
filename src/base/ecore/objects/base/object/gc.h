#ifndef ECO_OBJECTS_BASE_OBJECT_GC_H
#define ECO_OBJECTS_BASE_OBJECT_GC_H

#include "defs.h"

void Eco_Object_MarkChildren(struct Eco_GC_State*, struct Eco_Object*);
void Eco_Object_MarkInstance(struct Eco_GC_State*, struct Eco_Object*);

#endif
