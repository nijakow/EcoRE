#ifndef ECO_VM_BUILTINS_CORE_IS_H
#define ECO_VM_BUILTINS_CORE_IS_H

#include <ecore/vm/builtins/builtin.h>

bool Eco_VM_Builtin_Is(struct Eco_Fiber*, unsigned int);

bool Eco_VM_Builtin_CharAsInt(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_IntAsChar(struct Eco_Fiber*, unsigned int);

#endif
