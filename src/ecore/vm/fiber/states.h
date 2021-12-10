#ifndef ECO_VM_FIBER_STATES_H
#define ECO_VM_FIBER_STATES_H

#include <ecore/base/defs.h>

enum Eco_Fiber_State
{
    Eco_Fiber_State_RUNNING,
    Eco_Fiber_State_PAUSED,         // Doing nothing
    Eco_Fiber_State_WAITING,        // Waiting for a notification by some event
    Eco_Fiber_State_TERMINATED,
    Eco_Fiber_State_ERROR,
    Eco_Fiber_State_ERROR_BUILTIN_TRAP,
    Eco_Fiber_State_ERROR_NOOPCODE,
    Eco_Fiber_State_ERROR_ARGERROR,
    Eco_Fiber_State_ERROR_SENDFAILED,
    Eco_Fiber_State_ERROR_ASSIGNFAILED,
    Eco_Fiber_State_ERROR_ASFAILED,
    Eco_Fiber_State_ERROR_RETURNFAILED,
};

static inline bool Eco_Fiber_State_IsError(enum Eco_Fiber_State state)
{
    return state >= Eco_Fiber_State_ERROR;
}

#endif
