#ifndef ECO_VM_BUILTINS_CORE_STRING_H
#define ECO_VM_BUILTINS_CORE_STRING_H

#include <ecore/vm/builtins/builtin.h>

bool Eco_VM_Builtin_StringAtByte(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_StringStepAtByte(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_StringByteLength(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_StringCharLength(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_StringAsKey(struct Eco_Fiber*, unsigned int);

#endif
