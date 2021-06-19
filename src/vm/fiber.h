#pragma once

#include "../eco.h"


struct Eco_Frame;
struct Eco_Message;
struct Eco_Code;
struct Eco_Environment;


enum Eco_Fiber_State
{
    Eco_Fiber_State_RUNNING,
    Eco_Fiber_State_ERROR,
    Eco_Fiber_State_ERROR_NOOPCODE,
    Eco_Fiber_State_ERROR_ARGERROR,
    Eco_Fiber_State_ERROR_SENDFAILED,
    Eco_Fiber_State_ERROR_ASSIGNFAILED,
    Eco_Fiber_State_ERROR_RETURNFAILED,
};

struct Eco_Fiber
{
    enum Eco_Fiber_State    state;

    struct Eco_Fiber*       next;
    struct Eco_Fiber*       previous;
    struct Eco_Fiber*       next_in_queue;
    struct Eco_Fiber*       previous_in_queue;

    unsigned int            stack_size;
    unsigned int            stack_alloc_ptr;
    char                    stack[];
};

static inline void Eco_Fiber_SetState(struct Eco_Fiber* fiber, enum Eco_Fiber_State state)
{
    fiber->state = state;
}

static inline bool Eco_Fiber_HasTop(struct Eco_Fiber* fiber)
{
    return fiber->stack_size > 0;
}

static inline struct Eco_Frame* Eco_Fiber_Top(struct Eco_Fiber* fiber)
{
    return (struct Eco_Frame*) &(fiber->stack[fiber->stack_alloc_ptr]);
}

bool Eco_Fiber_Enter(struct Eco_Fiber*, struct Eco_Message*, struct Eco_Code*, struct Eco_Environment*);

void Eco_Fiber_Run(struct Eco_Fiber*);

