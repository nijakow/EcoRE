#ifndef ECO_VM_BUILTINS_CORE_BLOB_H
#define ECO_VM_BUILTINS_CORE_BLOB_H

#include <ecore/vm/builtins/builtin.h>

bool Eco_VM_Builtin_BlobNew(struct Eco_Fiber*, unsigned int);

bool Eco_VM_Builtin_BlobSize(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_BlobAtInt8(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_BlobAtInt16(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_BlobAtInt32(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_BlobAtPutInt8(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_BlobAtPutInt16(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_BlobAtPutInt32(struct Eco_Fiber*, unsigned int);

bool Eco_VM_Builtin_BlobReadFrom(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_BlobWriteTo(struct Eco_Fiber*, unsigned int);

bool Eco_VM_Builtin_BlobOpenFile(struct Eco_Fiber*, unsigned int);

#endif
