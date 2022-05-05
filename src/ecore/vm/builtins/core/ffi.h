#ifndef ECO_VM_BUILTINS_CORE_FFI_H
#define ECO_VM_BUILTINS_CORE_FFI_H

#include <ecore/vm/builtins/builtin.h>

#ifdef ECO_CONFIG_USE_FFI

bool Eco_VM_Builtin_FFIType_NewStruct(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_FFIType_NewUnion(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_FFIType_PointerTo(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_FFIType_PointeeOrSelf(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_FFIType_GetForIndex(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_FFIType_GetSizeInBytes(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_FFIType_GetTypeOf(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_FFIType_GetNameOf(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_FFIType_GetOffsetOf(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_FFIType_GetMemberCount(struct Eco_Fiber*, unsigned int);

bool Eco_VM_Builtin_FFIFunction_New(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_FFIFunction_ArgCount(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_FFIFunction_ReturnType(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_FFIFunction_ArgType(struct Eco_Fiber*, unsigned int);

bool Eco_VM_Builtin_FFIFunction_Call(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_FFIFunction_EcoCall(struct Eco_Fiber*, unsigned int);

bool Eco_VM_Builtin_FFIObject_Alloc(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_FFIObject_GetType(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_FFIObject_GetSize(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_FFIObject_AssignNull(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_FFIObject_Address(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_FFIObject_Cast(struct Eco_Fiber*, unsigned int);

#ifdef ECO_CONFIG_USE_DLOPEN
bool Eco_VM_Builtin_FFIObjectDLOpen(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_FFIObjectDLSym(struct Eco_Fiber*, unsigned int);
#endif

#endif

#endif
