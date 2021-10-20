#ifndef ECO_VM_BUILTINS_CORE_INIT_H
#define ECO_VM_BUILTINS_CORE_INIT_H

#include <ecore/vm/builtins/builtin.h>

bool Eco_VM_Builtin_SetLobby(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_SetBlockFamily(struct Eco_Fiber*, unsigned int);

#endif
