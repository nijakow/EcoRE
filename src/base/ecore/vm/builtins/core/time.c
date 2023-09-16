#include "string.h"

#include <ecore/vm/vm.h>

bool Eco_VM_Builtin_GetTimeSinceStart(struct Eco_Fiber* fiber, unsigned int args)
{
    struct timeval   current;
    struct timeval   diff;
    Eco_Floating     converted_time_in_seconds;
 
    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, 0))
        return false;
 
    gettimeofday(&current, NULL);

    timersub(&current, &fiber->vm->start_time, &diff);

    converted_time_in_seconds = (Eco_Floating)diff.tv_sec + ((Eco_Floating)diff.tv_usec / 1000000.0);

    Eco_Fiber_Push(fiber, Eco_Any_FromFloating(converted_time_in_seconds));

    return true;
}
