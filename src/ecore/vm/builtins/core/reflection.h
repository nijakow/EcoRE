#ifndef ECO_VM_BUILTINS_CORE_REFLECTION_H
#define ECO_VM_BUILTINS_CORE_REFLECTION_H

#include <ecore/vm/builtins/builtin.h>

bool Eco_VM_Builtin_GetType(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetTypeSlotNames(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetTypeSlotInfo(struct Eco_Fiber*, unsigned int);

bool Eco_VM_Builtin_InterfaceGetParents(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_InterfaceGetEntryInfo(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_InterfaceAddParent(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_InterfaceAddEntry(struct Eco_Fiber*, unsigned int);

#endif
