#ifndef ECO_OBJECTS_BASE_BUILTIN_H
#define ECO_OBJECTS_BASE_BUILTIN_H

#include "../../eco.h"

struct Eco_Fiber;

typedef bool (*Eco_Builtin)(struct Eco_Fiber*, unsigned int);

#endif
