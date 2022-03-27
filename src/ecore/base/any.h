#ifndef ECO_BASE_ANY_H
#define ECO_BASE_ANY_H

#include <ecore/base/defs.h>

/*
 *
 *    u n i o n   E c o _ V a l u e
 *
 */

typedef union eco_value
{
    void*         pointer;
    eco_unichar   character;
    eco_integer   integer;
    eco_floating  floating;
} eco_value;

typedef enum eco_value_type
{
    eco_value_type_pointer = 0,
    eco_value_type_nil,
    eco_value_type_bool,
    eco_value_type_unichar,
    eco_value_type_int,
    eco_value_type_float
} eco_value_type;


/*
 *
 *    s t r u c t  E c o _ A n y
 *
 */

typedef struct eco_any
{
    eco_value       value;
    eco_value_type  type;
} eco_any;


static inline eco_any eco_any_from_pointer(const void* ptr)
{
    return (eco_any) { .type = eco_value_type_pointer, .value.pointer = ptr };
}

static inline eco_any eco_any_nil()
{
    return (eco_any) { .type = eco_value_type_nil };
}

static inline eco_any eco_any_true()
{
    return (eco_any) { .type = eco_value_type_bool, .value.integer = 1 };
}

static inline eco_any eco_any_false()
{
    return (eco_any) { .type = eco_value_type_bool, .value.integer = 0 };
}

static inline eco_any eco_any_from_unichar(const eco_unichar unichar)
{
    return (eco_any) { .type = eco_value_type_unichar, .value.pointer = unichar };
}

static inline eco_any eco_any_from_int(const eco_int integer)
{
    return (eco_any) { .type = eco_value_type_int, .value.pointer = integer };
}

static inline eco_any eco_any_from_unichar(const eco_float floating)
{
    return (eco_any) { .type = eco_value_type_float, .value.pointer = floating };
}

static inline void* eco_any_as_pointer(const eco_any any)
{
    return any.value.pointer;
}

static inline bool eco_any_as_bool(const eco_any any)
{
    return (bool) any.value.integer;
}

static inline eco_int eco_any_as_int(const eco_any any)
{
    return any.value.integer;
}

static inline eco_float eco_any_as_float(const eco_any any)
{
    return any.value.floating;
}

static inline bool eco_any_is(const eco_any any, const eco_value_type type)
{
    return any.type == type;
}

#endif
