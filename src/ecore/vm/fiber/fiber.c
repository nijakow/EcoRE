#include "fiber.h"
#include "states.h"
#include "sched.h"
#include "stackops.h"

#include <ecore/vm/vm.h>
#include <ecore/vm/core/frame.h>
#include <ecore/vm/memory/memory.h>
#include <ecore/vm/memory/gc/gc.h>
#include <ecore/io/logging/log.h>


struct Eco_Fiber* Eco_Fiber_New(struct Eco_VM* vm, unsigned int stack_size)
{
    struct Eco_Fiber*  fiber;

    fiber = Eco_Memory_Alloc(sizeof(struct Eco_Fiber) + stack_size);

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

    Eco_Fiber_SetPaused(fiber);

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
