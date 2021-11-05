#ifndef ECO_VM_BUILTINS_CORE_INIT_H
#define ECO_VM_BUILTINS_CORE_INIT_H

#include <ecore/vm/builtins/builtin.h>

bool Eco_VM_Builtin_SetLobby(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_SetProxy(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetIntegerType(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetBlockType(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetVectorType(struct Eco_Fiber*, unsigned int);

#endif
