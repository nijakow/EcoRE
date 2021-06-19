#include "fiber.h"

#include "memory/memory.h"



struct Eco_Fiber* Eco_Fiber_New(struct Eco_VM* vm, unsigned int stack_size)
{
    struct Eco_Fiber*  fiber;

    fiber = Eco_Memory_Alloc(sizeof(struct Eco_Fiber) + stack_size);

    fiber->state = Eco_Fiber_State_PAUSED;

    fiber->vm = vm;
    fiber->queue = NULL;
    fiber->queue_prev = NULL;
    fiber->queue_next = NULL;

    fiber->stack_size = stack_size;
    fiber->stack_alloc_ptr = 0;
}

void Eco_Fiber_Delete(struct Eco_Fiber* fiber)
{
    Eco_Fiber_MoveToQueue(fiber, NULL);
    Eco_Memory_Free(fiber);
}

void Eco_Fiber_MoveToQueue(struct Eco_Fiber* fiber, struct Eco_Fiber** queue)
{
    if (fiber->queue != NULL) {
        if (*(fiber->queue) == fiber) {
            *(fiber->queue) = fiber->queue_next;
        }
        fiber->queue_prev->queue_next = fiber->queue_next;
        fiber->queue_next->queue_prev = fiber->queue_prev;
    }

    fiber->queue = queue;

    if (queue != NULL) {
        if (*queue == NULL) {
            fiber->queue_prev = fiber;
            fiber->queue_next = fiber;
        } else {
            fiber->queue_prev = (*queue)->queue_prev;
            fiber->queue_next = (*queue);
            fiber->queue_prev->queue_next = fiber;
            fiber->queue_next->queue_prev = fiber;
        }
        *queue = fiber;
    }
}
