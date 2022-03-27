#ifndef ECO_BASE_DEFS_H
#define ECO_BASE_DEFS_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "config.h"

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

typedef i32       eco_int;
typedef f32       eco_float;
typedef i32       eco_unichar;

struct eco_object;
struct eco_type;

#endif
