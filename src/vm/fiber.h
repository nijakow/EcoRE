#pragma once

#include "../eco.h"


struct Eco_Frame;
struct Eco_Message;
struct Eco_Code;
struct Eco_Environment;
struct Eco_VM;


enum Eco_Fiber_State
{
    Eco_Fiber_State_RUNNING,
    Eco_Fiber_State_PAUSED,
    Eco_Fiber_State_TERMINATED,
    Eco_Fiber_State_ERROR,
    Eco_Fiber_State_ERROR_NOOPCODE,
    Eco_Fiber_State_ERROR_ARGERROR,
    Eco_Fiber_State_ERROR_SENDFAILED,
    Eco_Fiber_State_ERROR_ASSIGNFAILED,
    Eco_Fiber_State_ERROR_RETURNFAILED,
};

static inline bool Eco_Fiber_State_IsError(enum Eco_Fiber_State state)
{
    return state >= Eco_Fiber_State_ERROR;
}

struct Eco_Fiber
{
    enum Eco_Fiber_State    state;

    struct Eco_VM*          vm;
    struct Eco_Fiber**      queue;
    struct Eco_Fiber*       queue_prev;
    struct Eco_Fiber*       queue_next;

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


struct Eco_Fiber* Eco_Fiber_New(struct Eco_VM*, unsigned int);
void Eco_Fiber_Delete(struct Eco_Fiber*);
void Eco_Fiber_MoveToQueue(struct Eco_Fiber*, struct Eco_Fiber**);
