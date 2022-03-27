#ifndef ECO_VM_BUILTINS_CORE_ARRAY_H
#define ECO_VM_BUILTINS_CORE_ARRAY_H

#include <ecore/vm/builtins/builtin.h>

bool Eco_VM_Builtin_ArrayNew(struct Eco_Fiber* fiber, unsigned int args);
bool Eco_VM_Builtin_ArraySize(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_ArrayAt(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_ArrayAtPut(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_ArrayToString(struct Eco_Fiber*, unsigned int);

#endif
