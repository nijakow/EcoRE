#pragma once

#include "../../eco.h"

#include "../../objects/object.h"



struct Eco_GC_State;


void Eco_GC_State_QueueObject(struct Eco_GC_State*, struct Eco_Object*);

static inline void Eco_GC_State_MarkObject(struct Eco_GC_State* state, struct Eco_Object* object)
{
    if (!object->header.mark_queued) {
        object->header.mark_queued = true;
        Eco_GC_State_QueueObject(state, object);
    }
}

static inline void Eco_GC_State_MarkAny(struct Eco_GC_State* state, Eco_Any* any)
{
    if (Eco_Any_IsPointer(any)) {
        Eco_GC_State_MarkObject(state, Eco_Any_AsPointer(any));
    }
}

void Eco_GC();
