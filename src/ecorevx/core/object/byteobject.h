#ifndef ECO_CORE_OBJECT_BYTEOBJECT_H
#define ECO_CORE_OBJECT_BYTEOBJECT_H

#include "object.h"


#define Eco_BYTE_OBJECT_SLOT_COUNT (Eco_DEFAULT_OBJECT_SLOT_COUNT)


static inline Eco_Any Eco_ByteObject_GetExtra(struct Eco_Object* self)
{
    return Eco_Object_Get(self, 0);
}

static inline void Eco_ByteObject_SetExtra(struct Eco_Object* self, Eco_Any value)
{
    return Eco_Object_Set(self, 0, value);
}

static inline char* Eco_ByteObject_GetBytePayload(struct Eco_Object* self)
{
    return (((char*) Eco_Object_GetPayload(self)) + sizeof(Eco_Any) * Eco_BYTE_OBJECT_SLOT_COUNT);
}

static inline char Eco_ByteObject_GetByte(struct Eco_Object* self, unsigned int index)
{
    return Eco_ByteObject_GetBytePayload(self)[index];
}

static inline void Eco_ByteObject_SetByte(struct Eco_Object* self, unsigned int index, char value)
{
    Eco_ByteObject_GetBytePayload(self)[index] = value;
}

static inline Eco_Size_t Eco_ByteObject_GetByteCount(struct Eco_Object* self)
{
    return Eco_Object_GetUnalignedSize(self) - sizeof(struct Eco_Object) - sizeof(Eco_Any) * Eco_BYTE_OBJECT_SLOT_COUNT;
}

struct Eco_Object* Eco_Object_AllocByteObject(Eco_Allocator_t, struct Eco_Map*, Eco_Size_t);

#endif
