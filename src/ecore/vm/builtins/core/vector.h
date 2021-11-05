#ifndef ECO_VM_BUILTINS_CORE_VECTOR_H
#define ECO_VM_BUILTINS_CORE_VECTOR_H

#include <ecore/vm/builtins/builtin.h>

bool Eco_VM_Builtin_VectorAt(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_VectorAtPut(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_VectorLength(struct Eco_Fiber*, unsigned int);

#endif
