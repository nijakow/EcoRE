#include "gc.h"

#include <ecore/vm/core/frame.h>
#include <ecore/vm/memory/memory.h>


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
