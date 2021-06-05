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

