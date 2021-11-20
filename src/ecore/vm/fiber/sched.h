#ifndef ECO_VM_FIBER_SCHED_H
#define ECO_VM_FIBER_SCHED_H

#include <ecore/eco.h>

#include "fiber.h"

struct Eco_FiberQueue
{
    struct Eco_Fiber*  fibers;
};

void Eco_FiberQueue_Create(struct Eco_FiberQueue*);
void Eco_FiberQueue_Destroy(struct Eco_FiberQueue*);


void Eco_Fiber_MoveToQueue(struct Eco_Fiber*, struct Eco_FiberQueue*);
void Eco_Fiber_SetRunning(struct Eco_Fiber*);
void Eco_Fiber_SetPaused(struct Eco_Fiber*);    // Deprecated
void Eco_Fiber_Pause(struct Eco_Fiber*);
void Eco_Fiber_ReactivateWithValue(struct Eco_Fiber*, Eco_Any*);
void Eco_Fiber_SetState(struct Eco_Fiber* fiber, enum Eco_Fiber_State state);   // Deprecated

#endif
