#ifndef ECO_VM_BUILTINS_CORE_PORT_H
#define ECO_VM_BUILTINS_CORE_PORT_H

#include <ecore/vm/builtins/builtin.h>

bool Eco_VM_Builtin_NewPort(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_PortFlushOutput(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_PortReadChar(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_PortWriteChar(struct Eco_Fiber*, unsigned int);

#endif
