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
    void*          pointer;
    Eco_Codepoint  character;
    Eco_Integer    integer;
    Eco_Floating   floating;
} Eco_Value;

typedef enum Eco_Value_Type
{
    Eco_Value_Type_POINTER = 0,
    Eco_Value_Type_CHARACTER,
    Eco_Value_Type_INTEGER,
    Eco_Value_Type_FLOATING
} Eco_Value_Type;


/*
 *
 *    s t r u c t  E c o _ A n y
 *
 */

#ifdef ECO_CONFIG_USE_ANY_UINTPTR

typedef uintptr_t Eco_Any;

#define Eco_Any_MASK            (((Eco_Any) 0x3) << 62)
#define Eco_Any_Mask_POINTER    (((Eco_Any) Eco_Value_Type_POINTER)   << 62)
#define Eco_Any_Mask_CHARACTER  (((Eco_Any) Eco_Value_Type_CHARACTER) << 62)
#define Eco_Any_Mask_INTEGER    (((Eco_Any) Eco_Value_Type_INTEGER)   << 62)
#define Eco_Any_Mask_FLOATING   (((Eco_Any) Eco_Value_Type_FLOATING)  << 62)


static inline bool Eco_Any_Equals(Eco_Any a, Eco_Any b)
{
    return a == b;
}

static inline enum Eco_Value_Type Eco_Any_GetValueType(Eco_Any any)
{
    return (enum Eco_Value_Type) ((any) >> 62);
}

static inline bool Eco_Any_IsPointer(Eco_Any any)
{
    return (((uintptr_t) any) & Eco_Any_MASK) == Eco_Any_Mask_POINTER;
}

static inline bool Eco_Any_IsCharacter(Eco_Any any)
{
    return ((uintptr_t) any & Eco_Any_MASK) == Eco_Any_Mask_CHARACTER;
}

static inline bool Eco_Any_IsInteger(Eco_Any any)
{
    return ((uintptr_t) any & Eco_Any_MASK) == Eco_Any_Mask_INTEGER;
}

static inline bool Eco_Any_IsFloating(Eco_Any any)
{
    return ((uintptr_t) any & Eco_Any_MASK) == Eco_Any_Mask_FLOATING;
}


static inline Eco_Any Eco_Any_FromPointer(void* pointer)
{
    return (Eco_Any) (uintptr_t) pointer;
}

static inline Eco_Any Eco_Any_FromCharacter(Eco_Codepoint character)
{
    uintptr_t  value;

    value = 0;
    value |= character & 0xffffffff;
    value |= Eco_Any_Mask_CHARACTER;
    return (Eco_Any) value;
}

static inline Eco_Any Eco_Any_FromInteger(Eco_Integer integer)
{
    uintptr_t  value;

    value = 0;
    value |= integer & 0xffffffff;
    value |= Eco_Any_Mask_INTEGER;
    return (Eco_Any) value;
}

static inline Eco_Any Eco_Any_FromFloating(Eco_Floating floating)
{
    union data
    {
        u32           i;
        Eco_Floating  f;
    } d;
    uintptr_t  value;

    d.f = floating;
    value = 0 | d.i;
    value |= Eco_Any_Mask_FLOATING;
    return (Eco_Any) value;
}

static inline void Eco_Any_AssignAny(Eco_Any* dest, Eco_Any* src)
{
    *dest = *src;
}


static inline void* Eco_Any_AsPointer(Eco_Any any)
{
    return (void*) any;
}

static inline Eco_Codepoint Eco_Any_AsCharacter(Eco_Any any)
{
    return (Eco_Codepoint) any;
}

static inline Eco_Integer Eco_Any_AsInteger(Eco_Any any)
{
    return (Eco_Integer) (any & 0xffffffff);
}

static inline Eco_Floating Eco_Any_AsFloating(Eco_Any any)
{
    union data
    {
        u32           i;
        Eco_Floating  f;
    } d;
    
    d.i = (any & 0xffffffff);
    return d.f;
}

#else

typedef struct Eco_Any
{
    Eco_Value       value;
    Eco_Value_Type  type;
} Eco_Any;


static inline enum Eco_Value_Type Eco_Any_GetValueType(Eco_Any any)
{
    return any.type;
}

static inline bool Eco_Any_Is(Eco_Any any, Eco_Value_Type type)
{
    return any.type == type;
}

static inline bool Eco_Any_IsPointer(Eco_Any any)
{
    return Eco_Any_Is(any, Eco_Value_Type_POINTER);
}

static inline bool Eco_Any_IsCharacter(Eco_Any any)
{
    return Eco_Any_Is(any, Eco_Value_Type_CHARACTER);
}

static inline bool Eco_Any_IsInteger(Eco_Any any)
{
    return Eco_Any_Is(any, Eco_Value_Type_INTEGER);
}

static inline bool Eco_Any_IsFloating(Eco_Any any)
{
    return Eco_Any_Is(any, Eco_Value_Type_FLOATING);
}


static inline Eco_Any Eco_Any_FromPointer(void* pointer)
{
    Eco_Any  any;

    any.value.pointer = pointer;
    any.type          = Eco_Value_Type_POINTER;

    return any;
}

static inline Eco_Any Eco_Any_FromInteger(Eco_Integer integer)
{
    Eco_Any  any;

    any.value.integer = integer;
    any.type          = Eco_Value_Type_INTEGER;

    return any;
}

static inline Eco_Any Eco_Any_FromCharacter(Eco_Codepoint character)
{
    Eco_Any  any;

    any.value.character = character;
    any.type            = Eco_Value_Type_CHARACTER;

    return any;
}

static inline Eco_Any Eco_Any_FromFloating(Eco_Floating floating)
{
    Eco_Any  any;

    any.value.floating = floating;
    any.type           = Eco_Value_Type_FLOATING;

    return any;
}

static inline void Eco_Any_AssignAny(Eco_Any* dest, Eco_Any* src)
{
    *dest = *src;
}


static inline void* Eco_Any_AsPointer(Eco_Any any)
{
    return any.value.pointer;
}

static inline Eco_Integer Eco_Any_AsInteger(Eco_Any any)
{
    return any.value.integer;
}

static inline Eco_Codepoint Eco_Any_AsCharacter(Eco_Any any)
{
    return any.value.character;
}

static inline Eco_Floating Eco_Any_AsFloating(Eco_Any any)
{
    return any.value.floating;
}

static inline bool Eco_Any_Equals(Eco_Any a, Eco_Any b)
{
    if (a.type != b.type)
        return false;
    switch (a.type)
    {
        case Eco_Value_Type_POINTER: return a.value.pointer == b.value.pointer;
        case Eco_Value_Type_CHARACTER:
        case Eco_Value_Type_INTEGER: return a.value.integer == b.value.integer;
        case Eco_Value_Type_FLOATING: return a.value.floating == b.value.floating;
        default: return false;
    }
}

#endif

static inline Eco_Any Eco_Any_Default()
{
    return Eco_Any_FromInteger(0);
}

#endif
