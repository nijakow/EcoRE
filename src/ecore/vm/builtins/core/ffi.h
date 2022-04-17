#ifndef ECO_VM_BUILTINS_CORE_FFI_H
#define ECO_VM_BUILTINS_CORE_FFI_H

#include <ecore/vm/builtins/builtin.h>

#ifdef ECO_CONFIG_USE_FFI

bool Eco_VM_Builtin_FFIType_GetForIndex(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_FFIType_GetSizeInBytes(struct Eco_Fiber*, unsigned int);

bool Eco_VM_Builtin_FFIFunction_New(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_FFIFunction_ArgCount(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_FFIFunction_ReturnType(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_FFIFunction_ArgType(struct Eco_Fiber*, unsigned int);

bool Eco_VM_Builtin_FFIFunction_Call(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_FFIFunction_EcoCall(struct Eco_Fiber*, unsigned int);

#endif

#endif
