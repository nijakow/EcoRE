#ifndef ECO_VM_BUILTINS_CORE_REFLECTION_H
#define ECO_VM_BUILTINS_CORE_REFLECTION_H

#include <ecore/vm/builtins/builtin.h>

bool Eco_VM_Builtin_IsMolecule(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_IsInterface(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_IsCode(struct Eco_Fiber*, unsigned int);

bool Eco_VM_Builtin_GetProxy(struct Eco_Fiber*, unsigned int);

bool Eco_VM_Builtin_GetSlotValue(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetSlotFlags(struct Eco_Fiber*, unsigned int);

bool Eco_VM_Builtin_GetType(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetTypeSlotNames(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetTypeSlotInfo(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetTypeSupers(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetTypeSubs(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetTypeProxy(struct Eco_Fiber*, unsigned int);

#endif
