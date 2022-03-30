#include "stackops.h"

#include <ecore/objects/vm/code/closure.h>
#include <ecore/vm/core/frame.h>


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

    for (i = argument_count; i < register_count; i++) {
        Eco_Any_Initialize(&arguments[i]);
    }

    the_frame->previous       = fiber->top;
    the_frame->return_to      = fiber->top;
    the_frame->lexical        = NULL;
    the_frame->closures       = NULL;
    the_frame->handler        = NULL;
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
