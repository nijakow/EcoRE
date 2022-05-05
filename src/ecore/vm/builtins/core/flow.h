#ifndef ECO_VM_BUILTINS_CORE_FLOW_H
#define ECO_VM_BUILTINS_CORE_FLOW_H

#include <ecore/vm/builtins/builtin.h>

bool Eco_VM_Builtin_Value(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_Reset(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_Throw(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_SetExceptionHandler(struct Eco_Fiber*, unsigned int);

#endif
