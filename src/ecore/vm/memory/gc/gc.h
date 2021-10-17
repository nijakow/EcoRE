#ifndef ECO_VM_MEMORY_GC_GC_H
#define ECO_VM_MEMORY_GC_GC_H

#include "gc_state.h"

#include <ecore/eco.h>
#include <ecore/objects/base/object.h>


static inline void Eco_GC_State_MarkObject(struct Eco_GC_State* state, void* object)
{
    struct Eco_Object*  the_object;

    the_object = (struct Eco_Object*) object;

    if (!the_object->header.mark_queued) {
        the_object->header.mark_queued = true;
        Eco_GC_State_QueueObject(state, the_object);
    }
}

static inline void Eco_GC_State_MarkAny(struct Eco_GC_State* state, Eco_Any* any)
{
    struct Eco_Object*  object;

    if (Eco_Any_IsPointer(any)) {
        object = Eco_Any_AsPointer(any);
        if (object != NULL)
            Eco_GC_State_MarkObject(state, object);
    }
}

void Eco_GC_MakeSticky(struct Eco_Object* object);

void Eco_GC_Step(struct Eco_GC_State*);
void Eco_GC_FreeAll(struct Eco_GC_State*);

#endif
