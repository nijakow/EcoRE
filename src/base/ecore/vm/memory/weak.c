#include "weak.h"

#include <ecore/io/logging/log.h>
#include <ecore/vm/memory/arena.h>


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

bool Eco_WeakObjectManager_RemoveObject(struct Eco_WeakObjectManager* self, struct Eco_Object** object)
{
    struct Eco_Object*  head;

    if (self->objects == NULL)
        return false;
    
    Eco_Log_Debug("Removing object %p from weak object manager %p\n", *object, self);
    
    head          = self->objects;
    self->objects = head->next;

    /*
     * We add the object to the main arena so that it can be collected
     * by the GC if it is not referenced anywhere else.
     */
    Eco_Arena_AddObject(&Eco_OBJECTS, head);

    return true;
}
