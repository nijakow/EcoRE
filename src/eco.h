#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


/*
 *
 *    T y p e   D e c l a r a t i o n s
 *
 */

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;

typedef int8_t    i8;
typedef int16_t   i16;
typedef int32_t   i32;

typedef float     f32;

typedef i32       Eco_Integer;
typedef f32       Eco_Floating;


/*
 *
 *    O b j e c t   a n d   T y p e   D e c l a r a t i o n s
 *
 */

struct Eco_Object;
struct Eco_Type;


/*
 *
 *    u n i o n   E c o _ V a l u e
 *
 */

typedef union Eco_Value
{
    struct Eco_Object*  pointer;
    Eco_Integer         integer;
    Eco_Floating        floating;
} Eco_Value;

typedef enum Eco_Value_Type
{
    Eco_Value_Type_POINTER = 0,
    Eco_Value_Type_INTEGER,
    Eco_Value_Type_FLOATING
} Eco_Value_Type;


/*
 *
 *    s t r u c t  E c o _ A n y
 *
 */

typedef struct Eco_Any
{
    Eco_Value       value;
    Eco_Value_Type  type;
} Eco_Any;


static inline void Eco_Any_AssignPointer(Eco_Any* any, struct Eco_Object* object)
{
    any->value.pointer = object;
    any->type          = Eco_Value_Type_POINTER;
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


static inline struct Eco_Object* Eco_Any_AsPointer(Eco_Any* any)
{
    return any->value.pointer;
}

static inline Eco_Integer Eco_Any_AsInteger(Eco_Any* any)
{
    return any->value.integer;
}

static inline Eco_Floating Eco_Any_AsFloating(Eco_Any* any)
{
    return any->value.floating;
}
