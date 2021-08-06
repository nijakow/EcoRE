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

    fiber->state            = Eco_Fiber_State_RUNNING;

    fiber->vm               = vm;
    fiber->queue            = NULL;
    fiber->queue_prev       = NULL;
    fiber->queue_next       = NULL;

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

    Eco_Memory_Free(fiber);
}

void Eco_Fiber_Mark(struct Eco_GC_State* state, struct Eco_Fiber* fiber)
{
    struct Eco_Frame*  frame;

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

struct Eco_Frame* Eco_Fiber_AllocFrame(struct Eco_Fiber* fiber,
                                       unsigned int argument_count,
                                       unsigned int fixed_argument_count,
                                       unsigned int register_count)
{
    Eco_Any*           arguments;
    struct Eco_Frame*  the_frame;
    unsigned int       i;

    const unsigned int frame_size = sizeof(struct Eco_Frame) + sizeof(Eco_Any) * register_count;

    arguments                 = Eco_Fiber_Nth(fiber, argument_count);

    the_frame                 = (struct Eco_Frame*) fiber->stack_pointer;
    the_frame->closures       = NULL;
    the_frame->previous       = fiber->top;
    the_frame->vararg_count   = argument_count - fixed_argument_count;   // TODO: Check underflows!
    the_frame->varargs        = &arguments[fixed_argument_count];
    the_frame->arguments      = arguments;
    the_frame->register_count = register_count;
    fiber->top                = the_frame;
    fiber->stack_pointer      = fiber->stack_pointer + frame_size;

    for (i = 0; i < fixed_argument_count; i++) {
        Eco_Any_AssignAny(&the_frame->registers[i], &arguments[i]);
    }

    return the_frame;
}

void Eco_Fiber_PopFrame(struct Eco_Fiber* fiber)
{
    Eco_Any*           result;
    struct Eco_Frame*  frame;

    frame  = Eco_Fiber_Top(fiber);
    result = Eco_Fiber_Nth(fiber, 1);

    while (frame->closures != NULL)
    {
        frame->closures->lexical = NULL;
        frame->closures->prev    = NULL;
        frame->closures = frame->closures->next;
    }

    fiber->top           = frame->previous;
    fiber->stack_pointer = (char*) frame->arguments;

    Eco_Fiber_Push(fiber, result);  // Re-push the returned value
}
