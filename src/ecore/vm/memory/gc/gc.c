#include "gc.h"


#include "ecore/io/logging/log.h"
#include "gc_state.h"

#include <ecore/vm/vm.h>
#include <ecore/vm/memory/memory.h>
#include <ecore/vm/builtins/builtins.h>
#include <ecore/objects/base/object.h>
#include <ecore/objects/base/type.h>


extern struct Eco_Arena* Eco_ARENAS;


void Eco_GC_MakeSticky(struct Eco_Object* object)
{
    /* TODO, FIXME, XXX: IMPLEMENT THIS! */
}

void Eco_GC_MarkRoots(struct Eco_GC_State* state)
{
    Eco_VM_Mark(state, &Eco_THE_VM);
    // TODO: Eco_GC_State_MarkObject(state, Eco_VM_Builtin_LOBBY);
}

void Eco_GC_MarkLoop(struct Eco_GC_State* state)
{
    struct Eco_Object* object;

    while (Eco_GC_State_HasObjectsToMark(state))
    {
        object = Eco_GC_State_NextObjectToMark(state);

        object->header.mark_done = true;
        object->type->typecore->mark(state, object);
    }
}

void Eco_GC_Mark(struct Eco_GC_State* state)
{
    Eco_GC_MarkRoots(state);
    Eco_GC_MarkLoop(state);
}

void Eco_GC_SweepArena(struct Eco_GC_State* state, struct Eco_Arena* arena)
{
    struct Eco_Object*  object;
    struct Eco_Object** ptr;

    ptr = &arena->objects;

    while (*ptr != NULL)
    {
        object = *ptr;

        if (object->header.mark_done) {
            object->header.mark_queued = false;
            object->header.mark_done   = false;
            ptr = &(object->next);
        } else {
            *ptr = object->next;
            object->type->typecore->del(object);
            arena->object_count--;
        }
    }
}

void Eco_GC_Sweep(struct Eco_GC_State* state)
{
    struct Eco_Arena*  arena;

    for (arena = Eco_ARENAS; arena != NULL; arena = arena->next)
    {
        Eco_GC_SweepArena(state, arena);
    }
}

void Eco_GC_Step(struct Eco_GC_State* state)
{
    Eco_GC_Mark(state);
    Eco_GC_Sweep(state);
}

void Eco_GC_FreeAll(struct Eco_GC_State* state)
{
    /*
     * TODO: Make sure that the mark list is empty
     */
    Eco_GC_Sweep(state);
}
