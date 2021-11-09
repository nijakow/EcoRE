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
    struct Eco_Object*  pointer;
    Eco_Codepoint       character;
    Eco_Integer         integer;
    Eco_Floating        floating;
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

#if 0

typedef struct Eco_Any
{
    Eco_Value       value;
    Eco_Value_Type  type;
} Eco_Any;


static inline bool Eco_Any_IsPointer(Eco_Any* any)
{
    return any->type == Eco_Value_Type_POINTER;
}

static inline bool Eco_Any_IsCharacter(Eco_Any* any)
{
    return any->type == Eco_Value_Type_CHARACTER;
}

static inline bool Eco_Any_IsInteger(Eco_Any* any)
{
    return any->type == Eco_Value_Type_INTEGER;
}

static inline bool Eco_Any_IsFloating(Eco_Any* any)
{
    return any->type == Eco_Value_Type_FLOATING;
}


static inline void Eco_Any_AssignPointer(Eco_Any* any, struct Eco_Object* pointer)
{
    any->value.pointer = pointer;
    any->type          = Eco_Value_Type_POINTER;
}

static inline void Eco_Any_AssignInteger(Eco_Any* any, Eco_Integer integer)
{
    any->value.integer = integer;
    any->type          = Eco_Value_Type_INTEGER;
}

static inline void Eco_Any_AssignCharacter(Eco_Any* any, Eco_Codepoint character)
{
    any->value.character = character;
    any->type            = Eco_Value_Type_CHARACTER;
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


static inline struct Eco_Object* Eco_Any_AsPointer(Eco_Any* any)
{
    return any->value.pointer;
}

static inline Eco_Integer Eco_Any_AsInteger(Eco_Any* any)
{
    return any->value.integer;
}

static inline Eco_Codepoint Eco_Any_AsCharacter(Eco_Any* any)
{
    return any->value.character;
}

static inline Eco_Floating Eco_Any_AsFloating(Eco_Any* any)
{
    return any->value.floating;
}

#else

typedef uintptr_t Eco_Any;

enum
{
    Eco_Any_Mask_POINTER   = 0x0000000000000000,
    Eco_Any_Mask_CHARACTER = 0xc000000000000000,
    Eco_Any_Mask_INTEGER   = 0x8000000000000000,
    Eco_Any_Mask_FLOATING  = 0x4000000000000000
};

#define Eco_Any_MASK 0xc000000000000000

static inline bool Eco_Any_IsPointer(Eco_Any* any)
{
    return (((uintptr_t) *any) & Eco_Any_MASK) == Eco_Any_Mask_POINTER;
}

static inline bool Eco_Any_IsCharacter(Eco_Any* any)
{
    return ((uintptr_t) *any & Eco_Any_MASK) == Eco_Any_Mask_CHARACTER;
}

static inline bool Eco_Any_IsInteger(Eco_Any* any)
{
    return ((uintptr_t) *any & Eco_Any_MASK) == Eco_Any_Mask_INTEGER;
}

static inline bool Eco_Any_IsFloating(Eco_Any* any)
{
    return ((uintptr_t) *any & Eco_Any_MASK) == Eco_Any_Mask_FLOATING;
}


static inline void Eco_Any_AssignPointer(Eco_Any* any, struct Eco_Object* pointer)
{
    *any = (uintptr_t) pointer;
}

static inline void Eco_Any_AssignCharacter(Eco_Any* any, Eco_Codepoint character)
{
    uintptr_t  value;

    value = (uintptr_t) character;
    value |= Eco_Any_Mask_CHARACTER;
    *any = (uintptr_t) value;
}

static inline void Eco_Any_AssignInteger(Eco_Any* any, Eco_Integer integer)
{
    uintptr_t  value;

    value = (uintptr_t) integer;
    value |= Eco_Any_Mask_INTEGER;
    *any = (uintptr_t) value;
}

static inline void Eco_Any_AssignFloating(Eco_Any* any, Eco_Floating floating)
{
    // TODO, FIXME, XXX!
}

static inline void Eco_Any_AssignAny(Eco_Any* dest, Eco_Any* src)
{
    *dest = *src;
}


static inline struct Eco_Object* Eco_Any_AsPointer(Eco_Any* any)
{
    return (struct Eco_Object*) *any;
}

static inline Eco_Codepoint Eco_Any_AsCharacter(Eco_Any* any)
{
    return (Eco_Codepoint) *any;
}

static inline Eco_Integer Eco_Any_AsInteger(Eco_Any* any)
{
    return (Eco_Integer) *any;
}

static inline Eco_Floating Eco_Any_AsFloating(Eco_Any* any)
{
    return 0.0f;    // TODO, FIXME, XXX!
}

#endif

static inline void Eco_Any_Initialize(Eco_Any* any)
{
    Eco_Any_AssignPointer(any, NULL);
}

#endif
