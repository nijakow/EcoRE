#ifndef ECO_BASE_DEFS_H
#define ECO_BASE_DEFS_H

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

typedef i32         Eco_Integer;
typedef f32         Eco_Floating;
typedef Eco_Integer Eco_Codepoint;

struct Eco_Object;
struct Eco_Type;

#endif
