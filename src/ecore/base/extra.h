#ifndef ECO_BASE_EXTRA_H
#define ECO_BASE_EXTRA_H

#include <ecore/base/any.h>
#include <ecore/objects/base/object.h>
#include <ecore/objects/base/type.h>


extern struct Eco_Type*  Eco_Integer_TYPE;

static inline struct Eco_Type* Eco_Any_GetType(Eco_Any* any)
{
    if (Eco_Any_IsPointer(any)) return Eco_Any_AsPointer(any)->type;
    else if (Eco_Any_IsInteger(any)) return Eco_Integer_TYPE;
    else return NULL;
}

#endif
