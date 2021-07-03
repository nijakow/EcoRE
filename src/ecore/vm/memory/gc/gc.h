#pragma once

#include "gc_state.h"

#include "../../../eco.h"
#include "../../../objects/object.h"


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
    if (Eco_Any_IsPointer(any)) {
        Eco_GC_State_MarkObject(state, Eco_Any_AsPointer(any));
    }
}

void Eco_GC_MakeSticky(struct Eco_Object* object);

void Eco_GC();
void Eco_FreeAll();
