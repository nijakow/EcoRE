#ifndef ECO_VM_BUILTINS_BUILTINS_H
#define ECO_VM_BUILTINS_BUILTINS_H

#include "../../eco.h"

extern struct Eco_Object* Eco_VM_Builtin_LOBBY;


void Eco_VM_Builtins_Init();
void Eco_VM_Builtins_Terminate();

#endif
