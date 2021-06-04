#pragma once

#include "../eco.h"

#include "frame.h"
#include "environment.h"


struct Eco_Fiber
{
    unsigned int       stack_size;
    unsigned int       stack_alloc_ptr;
    char               stack[];
};

static inline struct Eco_Frame* Eco_Fiber_Top(struct Eco_Fiber* fiber)
{
    return (struct Eco_Frame*) &(fiber->stack[fiber->stack_alloc_ptr]);
}

static inline struct Eco_Frame* Eco_Fiber_PushFrame(struct Eco_Fiber* fiber, unsigned int register_count)
{
    unsigned int      delta;
    struct Eco_Frame* new_frame;

    delta = sizeof(struct Eco_Frame) * Eco_Fiber_Top(fiber)->register_count * sizeof(Eco_Any); 

    fiber->stack_size += delta;

    new_frame = Eco_Fiber_Top(fiber);
    new_frame->delta = delta;
    new_frame->register_count = register_count;

    return new_frame;
}

static inline void Eco_Fiber_PopFrame(struct Eco_Fiber* fiber)
{
    struct Eco_Frame* frame;
    
    frame = Eco_Fiber_Top(fiber);

    if (frame->dynamic_vars != NULL) {
        Eco_Environment_Decr(frame->dynamic_vars);
    }

    fiber->stack_size -= frame->delta;
}
