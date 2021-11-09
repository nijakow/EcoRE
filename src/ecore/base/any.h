#ifndef ECO_BASE_ANY_H
#define ECO_BASE_ANY_H

#include <ecore/base/defs.h>

/*
 *
 *    u n i o n   E c o _ V a l u e
 *
 */

typedef union Eco_Value
{
    Eco_Ref       ref;
    Eco_Integer   integer;
    Eco_Floating  floating;
} Eco_Value;

typedef enum Eco_Value_Type
{
    Eco_Value_Type_REF = 0,
    Eco_Value_Type_CHARACTER,
    Eco_Value_Type_INTEGER,
    Eco_Value_Type_FLOATING
} Eco_Value_Type;


/*
 *
 *    s t r u c t  E c o _ A n y
 *
 */

#if 0

typedef struct Eco_Any
{
    Eco_Value       value;
    Eco_Value_Type  type;
} Eco_Any;


static inline bool Eco_Any_IsRef(Eco_Any* any)
{
    return any->type == Eco_Value_Type_REF;
}

static inline bool Eco_Any_IsInteger(Eco_Any* any)
{
    return any->type == Eco_Value_Type_INTEGER;
}

static inline bool Eco_Any_IsFloating(Eco_Any* any)
{
    return any->type == Eco_Value_Type_FLOATING;
}


static inline void Eco_Any_AssignRef(Eco_Any* any, Eco_Ref ref)
{
    any->value.ref = ref;
    any->type      = Eco_Value_Type_REF;
}

static inline void Eco_Any_AssignInteger(Eco_Any* any, Eco_Integer integer)
{
    any->value.integer = integer;
    any->type          = Eco_Value_Type_INTEGER;
}

static inline void Eco_Any_AssignFloating(Eco_Any* any, Eco_Floating floating)
{
    any->value.floating = floating;
    any->type           = Eco_Value_Type_FLOATING;
}

static inline void Eco_Any_AssignAny(Eco_Any* dest, Eco_Any* src)
{
    *dest = *src;
}


static inline struct Eco_Ref Eco_Any_AsRef(Eco_Any* any)
{
    return any->value.ref;
}

static inline Eco_Integer Eco_Any_AsInteger(Eco_Any* any)
{
    return any->value.integer;
}

static inline Eco_Floating Eco_Any_AsFloating(Eco_Any* any)
{
    return any->value.floating;
}

#else

typedef Eco_Ref Eco_Any;


static inline bool Eco_Any_IsRef(Eco_Any* any)
{
    return (((uintptr_t) *any) & 0x03) == Eco_Value_Type_REF;
}

static inline bool Eco_Any_IsCharacter(Eco_Any* any)
{
    return ((uintptr_t) *any & 0x03) == Eco_Value_Type_CHARACTER;
}

static inline bool Eco_Any_IsInteger(Eco_Any* any)
{
    return ((uintptr_t) *any & 0x03) == Eco_Value_Type_INTEGER;
}

static inline bool Eco_Any_IsFloating(Eco_Any* any)
{
    return ((uintptr_t) *any & 0x03) == Eco_Value_Type_FLOATING;
}


static inline void Eco_Any_AssignRef(Eco_Any* any, Eco_Ref ref)
{
    *any = ref;
}

static inline void Eco_Any_AssignCharacter(Eco_Any* any, Eco_Codepoint character)
{
    uintptr_t  value;

    value = (uintptr_t) character;
    value <<= 2;
    value |= Eco_Value_Type_CHARACTER & 0x03;
    *any = (struct Eco_Object*) value;
}

static inline void Eco_Any_AssignInteger(Eco_Any* any, Eco_Integer integer)
{
    uintptr_t  value;

    value = (uintptr_t) integer;
    value <<= 2;
    value |= Eco_Value_Type_INTEGER & 0x03;
    *any = (struct Eco_Object*) value;
}

static inline void Eco_Any_AssignFloating(Eco_Any* any, Eco_Floating floating)
{
    // TODO, FIXME, XXX!
}

static inline void Eco_Any_AssignAny(Eco_Any* dest, Eco_Any* src)
{
    *dest = *src;
}


static inline Eco_Ref Eco_Any_AsRef(Eco_Any* any)
{
    return *any;
}

static inline Eco_Codepoint Eco_Any_AsCharacter(Eco_Any* any)
{
    return (Eco_Codepoint) ((uintptr_t) *any >> 2);
}

static inline Eco_Integer Eco_Any_AsInteger(Eco_Any* any)
{
    return (Eco_Integer) ((uintptr_t) *any >> 2);
}

static inline Eco_Floating Eco_Any_AsFloating(Eco_Any* any)
{
    return 0.0f;    // TODO, FIXME, XXX!
}

#endif

static inline bool Eco_Any_IsPointer(Eco_Any* any)
{
    return Eco_Any_IsRef(any);
}

static inline struct Eco_Object* Eco_Any_AsPointer(Eco_Any* any)
{
    return Eco_DEREF(struct Eco_Object, Eco_Any_AsRef(any));
}

static inline void Eco_Any_AssignPointer(Eco_Any* any, struct Eco_Object* object)
{
    Eco_Any_AssignRef(any, Eco_REF(object));
}

static inline void Eco_Any_Initialize(Eco_Any* any)
{
    Eco_Any_AssignPointer(any, NULL);
}

#endif
