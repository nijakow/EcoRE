#ifndef ECO_VM_BUILTINS_CORE_SLOTS_H
#define ECO_VM_BUILTINS_CORE_SLOTS_H

#include <ecore/vm/builtins/builtin.h>

bool Eco_VM_Builtin_AddValueSlot(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_AddCodeSlot(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_RemoveSlot(struct Eco_Fiber*, unsigned int);

#endif
