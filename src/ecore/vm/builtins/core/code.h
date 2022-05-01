#ifndef ECO_VM_BUILTINS_CORE_CODE_H
#define ECO_VM_BUILTINS_CORE_CODE_H

#include <ecore/vm/builtins/builtin.h>

bool Eco_VM_Builtin_Code_New(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_Code_Value(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_Code_Bytecodes(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_Code_Constants(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_Code_Closures(struct Eco_Fiber*, unsigned int);

#endif
