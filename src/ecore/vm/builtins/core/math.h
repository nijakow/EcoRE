#ifndef ECO_VM_BUILTINS_CORE_MATH_H
#define ECO_VM_BUILTINS_CORE_MATH_H

#include <ecore/vm/builtins/builtin.h>

bool Eco_VM_Builtin_AddIntegers(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_Add(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_Subtract2(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_Multiply2(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_Divide2(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_Less2(struct Eco_Fiber*, unsigned int);

#endif
