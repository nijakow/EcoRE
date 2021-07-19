#pragma once

#include "../eco.h"

#include "fiber_states.h"


struct Eco_Frame;
struct Eco_Message;
struct Eco_Code;
struct Eco_Environment;
struct Eco_VM;
struct Eco_GC_State;


struct Eco_Fiber
{
    enum Eco_Fiber_State    state;

    struct Eco_VM*          vm;
    struct Eco_Fiber**      queue;
    struct Eco_Fiber*       queue_prev;
    struct Eco_Fiber*       queue_next;

    struct Eco_Frame*       top;
    unsigned int            stack_size;
    unsigned int            stack_alloc_ptr;
    char                    stack[];
};

static inline void Eco_Fiber_SetState(struct Eco_Fiber* fiber, enum Eco_Fiber_State state)
{
    fiber->state = state;
}

static inline struct Eco_Frame* Eco_Fiber_FrameAt(struct Eco_Fiber* fiber, unsigned int offset)
{
    return (struct Eco_Frame*) &(fiber->stack[offset]);
}

static inline struct Eco_Frame* Eco_Fiber_Top(struct Eco_Fiber* fiber)
{
    return fiber->top;
}

static inline bool Eco_Fiber_HasTop(struct Eco_Fiber* fiber)
{
    return Eco_Fiber_Top(fiber) != NULL;
}


struct Eco_Fiber* Eco_Fiber_New(struct Eco_VM*, unsigned int);
void Eco_Fiber_Delete(struct Eco_Fiber*);
void Eco_Fiber_Mark(struct Eco_GC_State*, struct Eco_Fiber*);
void Eco_Fiber_MoveToQueue(struct Eco_Fiber*, struct Eco_Fiber**);

struct Eco_Frame* Eco_Fiber_AllocFrame(struct Eco_Fiber*, unsigned int);
