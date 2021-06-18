#pragma once

#include "../eco.h"


struct Eco_Frame;
struct Eco_Message;
struct Eco_Code;
struct Eco_Environment;


struct Eco_Fiber
{
    struct Eco_Fiber*  next;
    struct Eco_Fiber*  previous;
    struct Eco_Fiber*  next_in_queue;
    struct Eco_Fiber*  previous_in_queue;

    unsigned int       stack_size;
    unsigned int       stack_alloc_ptr;
    char               stack[];
};

static inline struct Eco_Frame* Eco_Fiber_Top(struct Eco_Fiber* fiber)
{
    return (struct Eco_Frame*) &(fiber->stack[fiber->stack_alloc_ptr]);
}

bool Eco_Fiber_Enter(struct Eco_Fiber*, struct Eco_Message*, struct Eco_Code*, struct Eco_Environment*);
