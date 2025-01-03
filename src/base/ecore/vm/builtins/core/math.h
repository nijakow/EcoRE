#ifndef ECO_VM_BUILTINS_CORE_MATH_H
#define ECO_VM_BUILTINS_CORE_MATH_H

#include <ecore/vm/builtins/builtin.h>

bool Eco_VM_Builtin_IntAsFloat(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_FloatAsInt(struct Eco_Fiber*, unsigned int);

bool Eco_VM_Builtin_AddIntegers(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_Add(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_Subtract2(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_Multiply2(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_Divide2(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_Modulo2(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_BitOr2(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_BitAnd2(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_BitXor2(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_LShift2(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_RShift2(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_Less2(struct Eco_Fiber*, unsigned int);

bool Eco_VM_Builtin_Sqrt(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_Sin(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_Cos(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_Tan(struct Eco_Fiber*, unsigned int);

#endif
