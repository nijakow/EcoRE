#ifndef ECO_VM_FIBER_H
#define ECO_VM_FIBER_H

#include <ecore/eco.h>

#include "fiber_states.h"


struct Eco_Frame;
struct Eco_Message;
struct Eco_Code;
struct Eco_Environment;
struct Eco_VM;
struct Eco_GC_State;


struct Eco_Fiber
{
    enum Eco_Fiber_State  state;

    struct Eco_VM*        vm;
    struct Eco_Fiber**    queue;
    struct Eco_Fiber*     queue_prev;
    struct Eco_Fiber*     queue_next;

    struct Eco_Frame*     top;
    char*                 stack_pointer;
    char*                 stack_max;
    char                  stack[];
};

static inline void Eco_Fiber_SetState(struct Eco_Fiber* fiber, enum Eco_Fiber_State state)
{
    fiber->state = state;
}

static inline struct Eco_Frame* Eco_Fiber_Top(struct Eco_Fiber* fiber)
{
    return fiber->top;
}

static inline bool Eco_Fiber_HasTop(struct Eco_Fiber* fiber)
{
    return Eco_Fiber_Top(fiber) != NULL;
}

static inline bool Eco_Fiber_Push(struct Eco_Fiber* fiber, Eco_Any* src)
{
    Eco_Any_AssignAny((Eco_Any*) fiber->stack_pointer, src);
    fiber->stack_pointer += sizeof(Eco_Any);
    return true;
}

static inline bool Eco_Fiber_Pop(struct Eco_Fiber* fiber, Eco_Any* dest)
{
    fiber->stack_pointer -= sizeof(Eco_Any);
    Eco_Any_AssignAny(dest, (Eco_Any*) fiber->stack_pointer);
    return true;
}

static inline bool Eco_Fiber_Drop(struct Eco_Fiber* fiber)
{
    fiber->stack_pointer -= sizeof(Eco_Any);
    return true;
}

static inline bool Eco_Fiber_Dup(struct Eco_Fiber* fiber)
{
    Eco_Any_AssignAny((Eco_Any*) fiber->stack_pointer, (Eco_Any*) (fiber->stack_pointer - sizeof(Eco_Any)));
    fiber->stack_pointer += sizeof(Eco_Any);
    return true;
}

static inline Eco_Any* Eco_Fiber_Nth(struct Eco_Fiber* fiber, unsigned int n)
{
    return (Eco_Any*) (fiber->stack_pointer - n * sizeof(Eco_Any));
}


struct Eco_Fiber* Eco_Fiber_New(struct Eco_VM*, unsigned int);
void Eco_Fiber_Delete(struct Eco_Fiber*);
void Eco_Fiber_Mark(struct Eco_GC_State*, struct Eco_Fiber*);
void Eco_Fiber_MoveToQueue(struct Eco_Fiber*, struct Eco_Fiber**);

struct Eco_Frame* Eco_Fiber_AllocFrame(struct Eco_Fiber*, unsigned int, unsigned int, unsigned int);
void Eco_Fiber_PopFrame(struct Eco_Fiber*);

void Eco_Fiber_ResetFrame(struct Eco_Fiber*);

#endif
