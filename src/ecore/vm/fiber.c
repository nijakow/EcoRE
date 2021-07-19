#include "fiber.h"

#include "memory/memory.h"
#include "memory/gc/gc.h"

#include "core/frame.h"
#include "../objects/vm/code/closure.h"


struct Eco_Fiber* Eco_Fiber_New(struct Eco_VM* vm, unsigned int stack_size)
{
    struct Eco_Fiber*  fiber;

    fiber = Eco_Memory_Alloc(sizeof(struct Eco_Fiber) + stack_size);

    fiber->state           = Eco_Fiber_State_PAUSED;

    fiber->vm              = vm;
    fiber->queue           = NULL;
    fiber->queue_prev      = NULL;
    fiber->queue_next      = NULL;

    fiber->top             = NULL;
    fiber->stack_size      = stack_size;
    fiber->stack_alloc_ptr = 0;

    return fiber;
}

void Eco_Fiber_Delete(struct Eco_Fiber* fiber)
{
    while (Eco_Fiber_HasTop(fiber))
    {
        Eco_Fiber_PopFrame(fiber);
    }

    Eco_Fiber_MoveToQueue(fiber, NULL);

    Eco_Memory_Free(fiber);
}

void Eco_Fiber_Mark(struct Eco_GC_State* state, struct Eco_Fiber* fiber)
{
    unsigned int       offset;
    struct Eco_Frame*  frame;

    offset = fiber->stack_alloc_ptr;
    do
    {
        frame = Eco_Fiber_FrameAt(fiber, offset);

        Eco_Frame_Mark(state, frame);

        offset -= frame->delta;
    } while (frame->delta > 0);
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
    unsigned int       delta;

    const unsigned int frame_size = sizeof(struct Eco_Frame) + sizeof(Eco_Any) * register_count;

    the_frame                 = (struct Eco_Frame*) &fiber->stack[fiber->stack_alloc_ptr];
    the_frame->register_count = register_count;
    the_frame->closures       = NULL;

    if (Eco_Fiber_HasTop(fiber)) {
        delta = Eco_Fiber_Top(fiber) - the_frame;
    } else {
        delta = 0;
    }

    fiber->top = the_frame;
    fiber->stack_alloc_ptr += frame_size;

    Eco_Fiber_Top(fiber)->delta = delta;

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

    if (frame->delta == 0) {
        fiber->top = NULL;
    } else {
        fiber->stack_alloc_ptr = fiber->stack_alloc_ptr - frame->delta;
        fiber->top             = Eco_Fiber_FrameAt(fiber, fiber->stack_alloc_ptr);
    }
}

