#ifndef ECO_VM_MEMORY_WEAK_H
#define ECO_VM_MEMORY_WEAK_H

#include <ecore/objects/base/object.h>

struct Eco_WeakObjectManager
{
    struct Eco_Object*  objects;
};

void Eco_WeakObjectManager_Create(struct Eco_WeakObjectManager* self);
void Eco_WeakObjectManager_Destroy(struct Eco_WeakObjectManager* self);

#endif
