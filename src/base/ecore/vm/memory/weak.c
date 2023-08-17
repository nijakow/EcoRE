#include "weak.h"

#include <ecore/io/logging/log.h>

void Eco_WeakObjectManager_Create(struct Eco_WeakObjectManager* self)
{
    self->objects = NULL;
}

void Eco_WeakObjectManager_Destroy(struct Eco_WeakObjectManager* self)
{
    /*
     * TODO: Clear objects?
     */
    if (self->objects != NULL) {
        Eco_Log_Warning("Weak object manager was not cleared before destruction!\n");
    }
}


void Eco_WeakObjectManager_AddObject(struct Eco_WeakObjectManager* self, struct Eco_Object* object)
{
    Eco_Log_Debug("Adding object %p to weak object manager %p\n", object, self);
    object->next  = self->objects;
    self->objects = object;
}
