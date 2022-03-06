#ifndef ECO_VM_BUILTINS_CORE_BLOB_H
#define ECO_VM_BUILTINS_CORE_BLOB_H

#include <ecore/vm/builtins/builtin.h>

bool Eco_VM_Builtin_BlobNew(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_BlobSize(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_BlobAt(struct Eco_Fiber*, unsigned int);
bool Eco_VM_Builtin_BlobAtPutS8(struct Eco_Fiber*, unsigned int);

#endif
