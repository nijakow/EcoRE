#ifndef ECO_VM_BUILTINS_CORE_TIME_H
#define ECO_VM_BUILTINS_CORE_TIME_H

#include <ecore/vm/builtins/builtin.h>

bool Eco_VM_Builtin_GetTimeSinceStart(struct Eco_Fiber*, unsigned int);

#endif
