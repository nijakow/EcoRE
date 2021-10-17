#include <ecore/eco.h>
#include <ecore/io/logging/log.h>

#include "arena.h"


void Eco_Arena_Create(struct Eco_Arena* arena)
{
    arena->objects      = NULL;
    arena->object_count = 0;
}

void Eco_Arena_Destroy(struct Eco_Arena* arena)
{
    if (arena->objects != NULL || arena->object_count > 0)
        Eco_Log_Error("Tried to destroy an Eco_Arena with loaded objects!\n");
}
