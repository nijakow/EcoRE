#ifndef ECO_VM_BUILTINS_CORE_PORT_H
#define ECO_VM_BUILTINS_CORE_PORT_H

#include <ecore/vm/builtins/builtin.h>

bool Eco_VM_Builtin_NewPort(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_PortAtEOF(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_PortReadByte(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_PortFlushOutput(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_PortWriteByte(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_PortWriteChar(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_PortNotifyMe(struct Eco_Fiber*, unsigned int);

bool Eco_VM_Builtin_OpenFile(struct Eco_Fiber*, unsigned int);

#endif
