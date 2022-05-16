#ifndef ECO_OBJECTS_STRING_H
#define ECO_OBJECTS_STRING_H

#include "../../core/core.h"

struct Eco_Object* Eco_String_New(Eco_Allocator_t, const char*);

static inline Eco_Size_t Eco_String_GetByteLength(struct Eco_Object* self)
{
    /*
     * Return the byte size of the object, minus the string terminator.
     */
    return Eco_ByteObject_GetByteCount(self) - 1;
}

#endif
