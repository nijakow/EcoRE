#include "scheduler.h"

#include <ecore/vm/core/interpreter.h>
#include <ecore/io/logging/log.h>


void Eco_Scheduler_Create(struct Eco_Scheduler* scheduler)
{
    Eco_FiberQueue_Create(&scheduler->fiber_queues.running);
    Eco_FiberQueue_Create(&scheduler->fiber_queues.paused);
}

void Eco_Scheduler_Destroy(struct Eco_Scheduler* scheduler)
{
    Eco_FiberQueue_Destroy(&scheduler->fiber_queues.running);
    Eco_FiberQueue_Destroy(&scheduler->fiber_queues.paused);
}


void Eco_Scheduler_Run(struct Eco_Scheduler* scheduler)
{
    struct Eco_Fiber*  fiber;
    struct Eco_Fiber*  next;

    for (fiber = scheduler->fiber_queues.running.fibers;
         fiber != NULL;
         fiber = next)
    {
        next = fiber->next;

        Eco_Fiber_Run(fiber, 0x1000);

        if (fiber->state == Eco_Fiber_State_RUNNING) {
            /* Do nothing */
        } else if (Eco_Fiber_State_IsError(fiber->state)) {
            /* TODO: Catch error */
            Eco_Log_Error("Got a non-TERMINATED state: %d\n", fiber->state);
            Eco_Fiber_Delete(fiber);
        } else if (fiber->state == Eco_Fiber_State_TERMINATED) {
            Eco_Fiber_Delete(fiber);
        }
    }
}
