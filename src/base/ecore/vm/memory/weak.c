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
