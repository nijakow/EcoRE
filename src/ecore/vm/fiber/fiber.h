#ifndef ECO_VM_FIBER_FIBER_H
#define ECO_VM_FIBER_FIBER_H

#include <ecore/base/any.h>

#include "states.h"

struct Eco_Frame;
struct Eco_VM;

struct Eco_Fiber
{
    Eco_Any                 accu;

    enum Eco_Fiber_State    state;
    Eco_Any                 thrown;

    struct Eco_VM*          vm;
    struct Eco_Fiber**      prev;
    struct Eco_Fiber*       next;

    struct Eco_Scheduler*   scheduler;
    struct Eco_FiberQueue*  queue;
    struct Eco_Fiber**      queue_prev;
    struct Eco_Fiber*       queue_next;

    struct Eco_Frame*       top;
    char*                   stack_pointer;
    char*                   stack_max;
    char                    stack[];
};

struct Eco_Fiber* Eco_Fiber_New(struct Eco_VM*, unsigned int);
void Eco_Fiber_Delete(struct Eco_Fiber*);

static inline Eco_Any* Eco_Fiber_Accu(struct Eco_Fiber* fiber) {
    return &fiber->accu;
}

static inline Eco_Any Eco_Fiber_GetAccu(struct Eco_Fiber* fiber) {
    return fiber->accu;
}

static inline void Eco_Fiber_SetAccu(struct Eco_Fiber* fiber, Eco_Any value) {
    fiber->accu = value;
}

#endif
