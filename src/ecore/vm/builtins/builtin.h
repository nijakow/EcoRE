#ifndef ECO_VM_BUILTINS_BUILTIN_H
#define ECO_VM_BUILTINS_BUILTIN_H

#include "../../eco.h"

struct Eco_Fiber;

typedef bool (*Eco_Builtin)(struct Eco_Fiber*, unsigned int);

extern bool Eco_VM_Builtin_Trap(struct Eco_Fiber*, unsigned int);

#endif
