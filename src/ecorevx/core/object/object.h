#ifndef ECO_CORE_OBJECT_OBJECT_H
#define ECO_CORE_OBJECT_OBJECT_H

#include "../../util/memory.h"
#include "../any.h"

#define Eco_DEFAULT_OBJECT_SLOT_COUNT 1


enum Eco_ObjectType
{
    Eco_ObjectType_PLAIN,
    Eco_ObjectType_BYTES,
};

struct Eco_ObjectHeader
{
    /*
     * Information for the garbage collector:
     *    00: White
     *    01: Grey
     *    10: Moved
     *    11: Black
     */
    unsigned int  gc_bits : 2;

    /*
     * Stores an enum Eco_ObjectType.
     */
    unsigned int  type : 1;

    /*
     * The size of the object, in bytes.
     */
    unsigned int  size : 29;

    /*
     * The object's hash.
     */
    unsigned int  hash : 32;
};

struct Eco_Object
{
    struct Eco_ObjectHeader  header;
};

static inline Eco_Size_t Eco_Object_GetUnalignedSize(struct Eco_Object* self)
{
    return (Eco_Size_t) self->header.size;
}

static inline Eco_Size_t Eco_Object_GetAlignedSize(struct Eco_Object* self)
{
    return Eco_Util_StandardAlign(Eco_Object_GetUnalignedSize(self));
}

static inline unsigned int Eco_Object_GetSlotCount(struct Eco_Object* self)
{
    if (self->header.type == Eco_ObjectType_PLAIN)
        return (Eco_Object_GetUnalignedSize(self) - sizeof(struct Eco_Object)) % sizeof(Eco_Any);
    else
        return Eco_DEFAULT_OBJECT_SLOT_COUNT;
}

static inline void* Eco_Object_GetPayload(struct Eco_Object* self)
{
    return ((char*) self) + sizeof(struct Eco_Object);
}

static inline Eco_Any Eco_Object_Get(struct Eco_Object* self, unsigned int index)
{
    return ((Eco_Any*) Eco_Object_GetPayload(self))[index];
}

static inline void Eco_Object_Set(struct Eco_Object* self, unsigned int index, Eco_Any value)
{
    ((Eco_Any*) Eco_Object_GetPayload(self))[index] = value;
}

static inline char* Eco_Object_GetBytePayload(struct Eco_Object* self)
{
    return (((char*) Eco_Object_GetPayload(self)) + sizeof(Eco_Any) * Eco_DEFAULT_OBJECT_SLOT_COUNT);
}

static inline char Eco_Object_GetByte(struct Eco_Object* self, unsigned int index)
{
    return Eco_Object_GetBytePayload(self)[index];
}

static inline void Eco_Object_SetByte(struct Eco_Object* self, unsigned int index, char value)
{
    Eco_Object_GetBytePayload(self)[index] = value;
}

struct Eco_Object* Eco_Object_AllocByteObject(Eco_Allocator_t, Eco_Any, Eco_Size_t);

#endif
