#include "fiber.h"

#include <ecore/objects/vm/code/closure.h>
#include <ecore/vm/vm.h>
#include <ecore/vm/core/frame.h>
#include <ecore/vm/memory/memory.h>
#include <ecore/vm/memory/gc/gc.h>
#include <ecore/io/logging/log.h>


void Eco_FiberQueue_Create(struct Eco_FiberQueue* queue)
{
    queue->fibers = NULL;
}

void Eco_FiberQueue_Destroy(struct Eco_FiberQueue* queue)
{
    while (queue->fibers != NULL)
        Eco_Fiber_MoveToQueue(queue->fibers, NULL);
}


struct Eco_Fiber* Eco_Fiber_New(struct Eco_VM* vm, unsigned int stack_size)
{
    struct Eco_Fiber*  fiber;

    fiber = Eco_Memory_Alloc(sizeof(struct Eco_Fiber) + stack_size);

    fiber->state            = Eco_Fiber_State_RUNNING;

    fiber->vm               =  vm;
    fiber->prev             = &vm->fibers;
    fiber->next             =  vm->fibers;
    vm->fibers              =  fiber;

    fiber->scheduler        = &vm->scheduler;
    fiber->queue            =  NULL;
    fiber->queue_prev       =  NULL;
    fiber->queue_next       =  NULL;

    fiber->top              =  NULL;
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

    if (fiber->next != NULL)
        fiber->next->prev = fiber->prev;
    *(fiber->prev) = fiber->next;

    Eco_Memory_Free(fiber);
}

void Eco_Fiber_Mark(struct Eco_GC_State* state, struct Eco_Fiber* fiber)
{
    struct Eco_Frame*  frame;
    Eco_Any*           stack_pointer;
    Eco_Any*           limit;

    frame = fiber->top;
    limit = (Eco_Any*) fiber->stack_pointer;
    while (frame != NULL)
    {
        stack_pointer = (Eco_Any*) (frame + 1);
        while (stack_pointer < limit)
        {
            Eco_GC_State_MarkAny(state, stack_pointer);
            stack_pointer++;
        }
        Eco_Frame_Mark(state, frame);
        limit = (Eco_Any*) frame;
        frame = frame->previous;
    }
    stack_pointer = (Eco_Any*) fiber->stack;
    while (stack_pointer < limit)
    {
        Eco_GC_State_MarkAny(state, stack_pointer);
        stack_pointer++;
    }
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


struct Eco_Frame* Eco_Fiber_PushFrame(struct Eco_Fiber* fiber,
                                      unsigned int argument_count,
                                      unsigned int fixed_argument_count,
                                      unsigned int register_count)
{
    Eco_Any*           arguments;
    Eco_Any*           varargs;
    struct Eco_Frame*  the_frame;
    unsigned int       i;

    const unsigned int frame_size   = sizeof(struct Eco_Frame);
    const unsigned int vararg_count = (argument_count > fixed_argument_count) ? (argument_count - fixed_argument_count) : 0;

    arguments                 = Eco_Fiber_Nth(fiber, argument_count);
    the_frame                 = (struct Eco_Frame*) (((char*) arguments) + register_count * sizeof(Eco_Any));
    varargs                   = (Eco_Any*) (((char*) the_frame) + frame_size);
    fiber->stack_pointer      = (char*) &varargs[vararg_count];

    for (i = 0; i < vararg_count; i++) {
        Eco_Any_AssignAny(&varargs[i], &arguments[i + fixed_argument_count]);
    }

    the_frame->previous       = fiber->top;
    the_frame->lexical        = NULL;
    the_frame->closures       = NULL;
    the_frame->vararg_count   = vararg_count;
    the_frame->varargs        = varargs;
    the_frame->registers      = arguments;
    fiber->top                = the_frame;

    return the_frame;
}

void Eco_Fiber_PopFrame(struct Eco_Fiber* fiber)
{
    Eco_Any*             result;
    struct Eco_Frame*    frame;
    struct Eco_Closure*  closure;

    frame  = Eco_Fiber_Top(fiber);
    result = Eco_Fiber_Nth(fiber, 1);

    while (frame->closures != NULL)
    {
        closure          = frame->closures;
        frame->closures  = frame->closures->next;
        closure->lexical = NULL;
        closure->prev    = NULL;
        closure->next    = NULL;
    }

    fiber->top           = frame->previous;
    fiber->stack_pointer = (char*) frame->registers;

    Eco_Fiber_Push(fiber, result);  // Re-push the returned value
}

void Eco_Fiber_ResetFrame(struct Eco_Fiber* fiber)
{
    struct Eco_Frame*  top;

    top                  = Eco_Fiber_Top(fiber);

    top->instruction     = top->code->bytecodes;
    fiber->stack_pointer = (char*) &top->varargs[top->vararg_count];
}
