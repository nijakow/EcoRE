#ifndef ECO_OBJECTS_BASE_TYPE_INTERFACE_H
#define ECO_OBJECTS_BASE_TYPE_INTERFACE_H

#include "type.h"

struct Eco_Interface* Eco_Any_GetInterface(Eco_Any, bool);
struct Eco_Interface* Eco_Type_GetInterface(struct Eco_Type*, bool);

#endif
