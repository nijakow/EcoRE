#include "fiber.h"
#include "sched.h"
#include "stackops.h"

#include <ecore/eco.h>
#include <ecore/objects/io/port.h>
#include <ecore/vm/scheduler.h>


void Eco_FiberQueue_Create(struct Eco_FiberQueue* queue)
{
    queue->fibers = NULL;
}

void Eco_FiberQueue_Destroy(struct Eco_FiberQueue* queue)
{
    while (queue->fibers != NULL)
        Eco_Fiber_MoveToQueue(queue->fibers, NULL);
}


void Eco_Fiber_MoveToQueue(struct Eco_Fiber* fiber, struct Eco_FiberQueue* queue)
{
    if (fiber->queue != queue)
    {
        if (fiber->queue != NULL) {
            if (fiber->queue_next != NULL)
                fiber->queue_next->prev = fiber->queue_prev;
            *(fiber->queue_prev) = fiber->queue_next;
        }

        fiber->queue = queue;

        if (queue != NULL) {
            fiber->queue_prev = &queue->fibers;
            fiber->queue_next =  queue->fibers;
            queue->fibers     =  fiber;
        }
    }
}

void Eco_Fiber_SetRunning(struct Eco_Fiber* fiber)
{
    Eco_Fiber_MoveToQueue(fiber, &fiber->scheduler->fiber_queues.running);
}

void Eco_Fiber_SetPaused(struct Eco_Fiber* fiber)
{
    Eco_Fiber_MoveToQueue(fiber, &fiber->scheduler->fiber_queues.paused);
}

void Eco_Fiber_ReactivateWithValue(struct Eco_Fiber* fiber, Eco_Any* value)
{
    Eco_Fiber_Push(fiber, value);
    Eco_Fiber_SetRunning(fiber);
}

/*
 * TODO, FIXME, XXX: Deprecate this!
 */
void Eco_Fiber_SetState(struct Eco_Fiber* fiber, enum Eco_Fiber_State state)
{
    fiber->state = state;
}
