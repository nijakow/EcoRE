#ifndef ECO_VM_BUILTINS_CORE_WEAK_H
#define ECO_VM_BUILTINS_CORE_WEAK_H

#include <ecore/vm/builtins/builtin.h>

bool Eco_VM_Builtin_WeakBoxNew(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_WeakBoxGet(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_WeakBoxSet(struct Eco_Fiber*, unsigned int);

#endif
