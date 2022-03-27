#ifndef ECO_BASE_EXTRA_H
#define ECO_BASE_EXTRA_H

#include <ecore/base/any.h>
#include <ecore/objects/base/object.h>
#include <ecore/objects/base/type.h>


extern struct Eco_Type*  Eco_Integer_TYPE;
extern struct Eco_Type*  Eco_Character_TYPE;

static inline struct Eco_Type* Eco_Any_GetType(Eco_Any* any)
{
    if (Eco_Any_IsPointer(*any)) return ((struct Eco_Object*) Eco_Any_AsPointer(*any))->type;
    else if (Eco_Any_IsInteger(*any)) return Eco_Integer_TYPE;
    else if (Eco_Any_IsCharacter(*any)) return Eco_Character_TYPE;
    else return NULL;
}

#endif
