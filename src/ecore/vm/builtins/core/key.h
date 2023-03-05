#ifndef ECO_VM_BUILTINS_CORE_KEY_H
#define ECO_VM_BUILTINS_CORE_KEY_H

#include <ecore/vm/builtins/builtin.h>

bool Eco_VM_Builtin_KeyAsString(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_Gensym(struct Eco_Fiber*, unsigned int);

#endif
