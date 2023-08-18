#ifndef ECO_VM_BUILTINS_CORE_INIT_H
#define ECO_VM_BUILTINS_CORE_INIT_H

#include <ecore/vm/builtins/builtin.h>

bool Eco_VM_Builtin_GetVersionString(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetOsVersionId(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetEnv(struct Eco_Fiber*, unsigned int);

bool Eco_VM_Builtin_SetLobby(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_SetTrue(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_SetFalse(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_SetBasicError(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_SetProxy(struct Eco_Fiber*, unsigned int);

bool Eco_VM_Builtin_GetTypeType(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetArrayType(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetBlobType(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetCodeType(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetIntegerType(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetFloatType(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetCharacterType(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetBlockType(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetKeyType(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetStringType(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetInterfaceType(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetDefaultInterface(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetWeakBoxType(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetFFITypeType(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetFFIObjectType(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetFFIFuncType(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_GetFFILibType(struct Eco_Fiber*, unsigned int);

#endif
