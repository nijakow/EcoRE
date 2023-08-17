#ifndef ECO_VM_MEMORY_WEAK_H
#define ECO_VM_MEMORY_WEAK_H

#include <ecore/objects/base/object.h>

struct Eco_GC_State;


struct Eco_WeakObjectManager
{
    struct Eco_Object*  objects;
};

void Eco_WeakObjectManager_Create(struct Eco_WeakObjectManager* self);
void Eco_WeakObjectManager_Destroy(struct Eco_WeakObjectManager* self);

void Eco_WeakObjectManager_AddObject(struct Eco_WeakObjectManager* self, struct Eco_Object* object);
bool Eco_WeakObjectManager_RemoveObject(struct Eco_WeakObjectManager* self, struct Eco_Object** object);

void Eco_WeakObjectManager_Mark(struct Eco_GC_State* state, struct Eco_WeakObjectManager* self);

#endif
