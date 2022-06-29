#include "stackops.h"

#include <ecore/objects/vm/code/closure.h>
#include <ecore/vm/core/frame.h>


struct Eco_Frame* Eco_Fiber_PushFrame(struct Eco_Fiber* fiber,
                                      Eco_Any      myself,
                                      unsigned int argument_count,
                                      unsigned int fixed_argument_count,
                                      unsigned int register_count)
{
    struct Eco_Frame*  the_frame;
    Eco_Any*           the_args;
    Eco_Any*           the_regs;
    unsigned int       i;

    the_args  = (Eco_Any*) fiber->stack_pointer;
    the_regs  = Eco_Fiber_StackAlloc(fiber, sizeof(Eco_Any) * register_count);
    the_frame = Eco_Fiber_StackAlloc(fiber, sizeof(struct Eco_Frame));

    for (i = 0; i < register_count; i++) {
        Eco_Any_Initialize(&the_regs[i]);
    }

    the_frame->previous        = fiber->top;
    the_frame->return_to       = fiber->top;
    the_frame->lexical         = NULL;
    the_frame->closures        = NULL;
    the_frame->handler         = NULL;
    the_frame->myself          = myself;
    the_frame->named_arg_count = fixed_argument_count;
    the_frame->args            = the_args;
    the_frame->registers       = the_regs;
    fiber->top                 = the_frame;

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
    fiber->stack_pointer = ((char*) top) + sizeof(struct Eco_Frame);
}
