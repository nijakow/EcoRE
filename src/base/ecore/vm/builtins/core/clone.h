#ifndef ECO_VM_BUILTINS_CORE_CLONE_H
#define ECO_VM_BUILTINS_CORE_CLONE_H

#include <ecore/vm/builtins/builtin.h>

bool Eco_VM_Builtin_Clone(struct Eco_Fiber*, unsigned int);

bool Eco_VM_Builtin_EnableFinalization(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_NextFinalizableOr(struct Eco_Fiber*, unsigned int);

#endif
