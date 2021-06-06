#include "fiber.h"

#include "bytecodes.h"
#include "code.h"
#include "send.h"


struct Eco_Frame* Eco_Fiber_PushFrame(struct Eco_Fiber*        fiber,
                                      unsigned int             register_count,
                                      unsigned int             dynamics_count,
                                      struct Eco_Environment*  link)
{
    unsigned int       delta;
    struct Eco_Frame*  new_frame;

    delta = sizeof(struct Eco_Frame) * Eco_Fiber_Top(fiber)->register_count * sizeof(Eco_Any); 

    fiber->stack_size += delta;

    new_frame = Eco_Fiber_Top(fiber);

    new_frame->delta = delta;
    new_frame->register_count = register_count;

    /* TODO: Allocate dynamics */

    return new_frame;
}

void Eco_Fiber_PopFrame(struct Eco_Fiber* fiber)
{
    struct Eco_Frame*  frame;
    
    frame = Eco_Fiber_Top(fiber);

    if (frame->dynamic_vars != NULL) {
        Eco_Environment_Decr(frame->dynamic_vars);
    }

    fiber->stack_size -= frame->delta;
}


bool Eco_Fiber_Enter(struct Eco_Fiber*        fiber,
                     struct Eco_Message*      message,
                     struct Eco_Code*         code,
                     struct Eco_Environment*  link)
{
    unsigned int       i;
    struct Eco_Frame*  frame;

    if (code->arg_count != (message->arg_count + 1)) {  /* The +1 stands for the SELF value */
        /* TODO: Error */
        return false;
    }

    frame = Eco_Fiber_PushFrame(fiber, code->register_count, code->dynamics_count, link);

    frame->self        = message->arg_location[0];

    frame->code        = code;
    frame->instruction = code->bytecodes;

    for (i = 0; i < code->arg_count; i++)
    {
        frame->registers[i] = message->arg_location[i + 1];
    }

    return true;
}


void Eco_Fiber_Run(struct Eco_Fiber* fiber)
{
    struct Eco_Frame*  top;

    top = Eco_Fiber_Top(fiber);

    while (true)
    {
        switch (Eco_Frame_NextU8(top))
        {
            case Eco_Bytecode_NOOP: {
                break;
            }
            case Eco_Bytecode_SELF: {
                u8 reg = Eco_Frame_NextU8(top);
                top->registers[reg] = top->self;
                break;
            }
            case Eco_Bytecode_R2R: {
                u8 to   = Eco_Frame_NextU8(top);
                u8 from = Eco_Frame_NextU8(top);
                top->registers[to] = top->registers[from];
                break;
            }
            case Eco_Bytecode_R2D: {
                u8 to   = Eco_Frame_NextU8(top);
                u8 from = Eco_Frame_NextU8(top);
                top->dynamic_vars->vars[to] = top->registers[from];
                break;
            }
            case Eco_Bytecode_D2R: {
                u8 to   = Eco_Frame_NextU8(top);
                u8 from = Eco_Frame_NextU8(top);
                top->registers[to] = top->dynamic_vars->vars[from];
                break;
            }
            case Eco_Bytecode_SEND: {
                struct Eco_Message  message;

                message.arg_location = &(top->registers[Eco_Frame_NextU8(top)]);
                message.arg_count    = Eco_Frame_NextU8(top);
                message.key          = Eco_Any_AsPointer(Eco_Frame_NextConstant(top));
                message.fiber        = fiber;

                if (!Eco_Send(&message, message.arg_location)) {
                    top = Eco_Fiber_Top(fiber); /* TODO: Do the "slow dispatch" code */
                } else {
                    /* TODO: Error */
                }

                break;
            }
            default: {
                /* TODO: Error */
                break;
            }
        }
    }
}

