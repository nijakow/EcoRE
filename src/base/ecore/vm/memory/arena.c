#include <ecore/eco.h>
#include <ecore/objects/base/object.h>
#include <ecore/io/logging/log.h>

#include "arena.h"

struct Eco_Arena* Eco_ARENAS = NULL;


void Eco_Arena_Create(struct Eco_Arena* arena)
{
    arena->objects        = NULL;
    arena->object_count   = 0;
    arena->prev           = &Eco_ARENAS;
    arena->next           =  Eco_ARENAS;
    if (arena->next != NULL)
        arena->next->prev = &arena->next;
    Eco_ARENAS            = arena;
}

void Eco_Arena_Destroy(struct Eco_Arena* arena)
{
    if (arena->objects != NULL || arena->object_count > 0)
        Eco_Log_Error("Tried to destroy an Eco_Arena with %u loaded objects (%p)!\n",
                      arena->object_count,
                      arena->objects);
    if (arena->next != NULL)
        arena->next->prev = arena->prev;
    *(arena->prev)        = arena->next;
}

void Eco_Arena_AddObject(struct Eco_Arena* arena, struct Eco_Object* object)
{
    object->next   = arena->objects;
    arena->objects = object;
    arena->object_count++;
}
