#include "fiber.h"

#include "memory/memory.h"
#include "memory/gc/gc.h"

#include "core/frame.h"
#include "../objects/vm/code/closure.h"

#include "../io/logging/log.h"


struct Eco_Fiber* Eco_Fiber_New(struct Eco_VM* vm, unsigned int stack_size)
{
    struct Eco_Fiber*  fiber;

    fiber = Eco_Memory_Alloc(sizeof(struct Eco_Fiber) + stack_size);

    fiber->state            = Eco_Fiber_State_PAUSED;

    fiber->vm               = vm;
    fiber->queue            = NULL;
    fiber->queue_prev       = NULL;
    fiber->queue_next       = NULL;

    fiber->data_stack       = Eco_Memory_Alloc(1024 * sizeof(Eco_Any));
    fiber->data_stack_alloc = 0;
    fiber->data_stack_size  = 1024;

    fiber->top              = NULL;
    fiber->stack_pointer    = &fiber->stack[0];
    fiber->stack_max        = &fiber->stack[stack_size];

    return fiber;
}

void Eco_Fiber_Delete(struct Eco_Fiber* fiber)
{
    while (Eco_Fiber_HasTop(fiber))
    {
        Eco_Fiber_PopFrame(fiber);
    }

    Eco_Fiber_MoveToQueue(fiber, NULL);

    Eco_Memory_Free(fiber->data_stack);
    Eco_Memory_Free(fiber);
}

void Eco_Fiber_Mark(struct Eco_GC_State* state, struct Eco_Fiber* fiber)
{
    unsigned int       offset;
    struct Eco_Frame*  frame;

    for (offset = 0; offset < fiber->data_stack_alloc; offset++)
    {
        Eco_GC_State_MarkAny(state, &fiber->data_stack[offset]);
    }

    frame = fiber->top;
    while (frame != NULL)
    {
        Eco_Frame_Mark(state, frame);
        frame = frame->previous;
    }
}

void Eco_Fiber_MoveToQueue(struct Eco_Fiber* fiber, struct Eco_Fiber** queue)
{
    if (fiber->queue != NULL) {
        if (*(fiber->queue) == fiber) {
            if (fiber->queue_next == fiber) {
                *(fiber->queue) = NULL;
            } else {
                *(fiber->queue) = fiber->queue_next;
            }
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

struct Eco_Frame* Eco_Fiber_AllocFrame(struct Eco_Fiber* fiber, unsigned int register_count)
{
    struct Eco_Frame*  the_frame;

    const unsigned int frame_size = sizeof(struct Eco_Frame) + sizeof(Eco_Any) * register_count;

    the_frame                 = (struct Eco_Frame*) fiber->stack_pointer;
    the_frame->register_count = register_count;
    the_frame->closures       = NULL;
    the_frame->previous       = fiber->top;
    fiber->top                = the_frame;
    fiber->stack_pointer      = fiber->stack_pointer + frame_size;

    return the_frame;
}

void Eco_Fiber_PopFrame(struct Eco_Fiber* fiber)
{
    struct Eco_Frame*  frame;

    frame = Eco_Fiber_Top(fiber);

    while (frame->closures != NULL)
    {
        frame->closures->lexical = NULL;
        frame->closures->prev    = NULL;
        frame->closures = frame->closures->next;
    }

    fiber->top           = frame->previous;
    fiber->stack_pointer = (char*) frame;
}
