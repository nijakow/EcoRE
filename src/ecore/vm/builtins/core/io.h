#ifndef ECO_VM_BUILTINS_IO_H
#define ECO_VM_BUILTINS_IO_H

#include <ecore/vm/builtins/builtin.h>

bool Eco_VM_Builtin_Print(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_OpenFile(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_FileExists(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_FileIsDirectory(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_ListFiles(struct Eco_Fiber*, unsigned int);

#endif
