#ifndef ECO_VM_FIBER_SCHED_H
#define ECO_VM_FIBER_SCHED_H

#include <ecore/eco.h>


struct Eco_Fiber;

struct Eco_FiberQueue
{
    struct Eco_Fiber*  fibers;
};

void Eco_FiberQueue_Create(struct Eco_FiberQueue*);
void Eco_FiberQueue_Destroy(struct Eco_FiberQueue*);


enum Eco_Fiber_State
{
    Eco_Fiber_State_RUNNING,
    Eco_Fiber_State_PAUSED,
    Eco_Fiber_State_TERMINATED,
    Eco_Fiber_State_ERROR,
    Eco_Fiber_State_ERROR_BUILTIN_TRAP,
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

void Eco_Fiber_MoveToQueue(struct Eco_Fiber*, struct Eco_FiberQueue*);
void Eco_Fiber_SetRunning(struct Eco_Fiber*);
void Eco_Fiber_SetPaused(struct Eco_Fiber*);

#endif
