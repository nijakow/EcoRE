#ifndef ECO_VM_BUILTINS_CORE_REFLECTION_H
#define ECO_VM_BUILTINS_CORE_REFLECTION_H

#include <ecore/vm/builtins/builtin.h>

bool Eco_VM_Builtin_GetType(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetTypeSlotNames(struct Eco_Fiber*, unsigned int);

bool Eco_VM_Builtin_InterfaceGetEntryNames(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_InterfaceAddEntry(struct Eco_Fiber*, unsigned int);

#endif
