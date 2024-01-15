#ifndef ECO_OBJECTS_STRING_H
#define ECO_OBJECTS_STRING_H

#include "../../core/core.h"

struct Eco_Object* Eco_String_New(Eco_Allocator_t, const char*);

/*
 * Return the byte size of the string, minus the string terminator.
 */
static inline Eco_Size_t Eco_String_GetByteLength(struct Eco_Object* self)
{
    return Eco_ByteObject_GetByteCount(self) - 1;
}

/*
 * Return the size of the string (in characters).
 */
static inline Eco_Size_t Eco_String_GetCharacterCount(struct Eco_Object* self)
{
    return (Eco_Size_t) Eco_Any_AsInteger(Eco_ByteObject_GetExtra(self));
}

/*
 * Set the size of the string.
 */
static inline void Eco_String_SetCharacterCount(struct Eco_Object* self, Eco_Size_t value)
{
    return Eco_ByteObject_SetExtra(self, Eco_Any_FromInteger(value));
}

#endif
