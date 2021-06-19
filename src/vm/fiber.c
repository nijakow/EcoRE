#include "fiber.h"

#include "../objects/code.h"
#include "../objects/closure.h"

#include "bytecodes.h"
#include "environment.h"
#include "frame.h"
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

    new_frame->delta          = delta;
    new_frame->register_count = register_count;

    if (dynamics_count > 0 || link != NULL) {
        new_frame->dynamic_vars = Eco_Environment_New(dynamics_count, link);
    } else {
        new_frame->dynamic_vars = NULL;
    }

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

    if (code->arg_count != (message->body.send.arg_count + 1)) {  /* The +1 stands for the SELF value */
        Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_ARGERROR);
        return false;
    }

    frame = Eco_Fiber_PushFrame(fiber, code->register_count, code->dynamics_count, link);

    frame->self        = message->body.send.arg_location[0];

    frame->code        = code;
    frame->instruction = code->bytecodes;

    for (i = 0; i < code->arg_count; i++)
    {
        frame->registers[i] = message->body.send.arg_location[i + 1];
    }

    return true;
}


void Eco_Fiber_Run(struct Eco_Fiber* fiber)
{
    struct Eco_Frame*  top;

    top = Eco_Fiber_Top(fiber);

    while (true)  /* TODO: Execution limit */
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

                message.body.send.arg_location = &(top->registers[Eco_Frame_NextU8(top)]);
                message.body.send.arg_count    = Eco_Frame_NextU8(top);
                message.key                    = Eco_Any_AsPointer(Eco_Frame_NextConstant(top));
                message.fiber                  = fiber;
                message.type                   = Eco_Message_Type_SEND;

                if (Eco_Send(&message, message.body.send.arg_location)) {
                    top = Eco_Fiber_Top(fiber); /* TODO: Do the "slow dispatch" code */
                } else {
                    Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_SENDFAILED);
                }

                break;
            }
            case Eco_Bytecode_ASSIGN: {
                struct Eco_Message  message;
                u8                  reg;

                reg                       = Eco_Frame_NextU8(top);
                message.body.assign.value = &(top->registers[Eco_Frame_NextU8(top)]);
                message.key               = Eco_Any_AsPointer(Eco_Frame_NextConstant(top));
                message.fiber             = fiber;
                message.type              = Eco_Message_Type_ASSIGN;

                if (!Eco_Send(&message, &(top->registers[reg]))) {
                    Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_ASSIGNFAILED);
                }

                break;
            }
            case Eco_Bytecode_RETURN: {
                u8                       i;
                u8                       depth;
                struct Eco_Environment*  next;

                depth = Eco_Frame_NextU8(top);

                if (depth > 0) {
                    i    = 0;
                    next = top->dynamic_vars;

                    while (i <= depth)  /* Since we start with next = top->dynamic_vars, we have to use "<=" */
                    {
                        if (next == NULL) {
                            Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_RETURNFAILED);
                            goto error;
                        }

                        if (Eco_Fiber_Top(fiber)->dynamic_vars == next) {
                            i    = i + 1;
                            next = next->link;
                        } else {
                            Eco_Fiber_PopFrame(fiber);
                            if (!Eco_Fiber_HasTop(fiber)) {
                                Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_RETURNFAILED);
                                goto error;
                            }
                        }
                    }
                }

                Eco_Fiber_PopFrame(fiber);

                top = Eco_Fiber_Top(fiber); /* TODO: Do the "slow dispatch" code */

                break;
            }
            case Eco_Bytecode_MAKE_CLOSURE: {
                u8                   dest;
                u8                   closure_id;
                struct Eco_Closure*  closure;

                dest       = Eco_Frame_NextU8(top);
                closure_id = Eco_Frame_NextU8(top);
                closure    = Eco_Closure_New(top->code->code_instances[closure_id], top->dynamic_vars);

                Eco_Any_AssignPointer(&top->registers[dest], (struct Eco_Object*) closure);

                break;
            }
            default: {
                Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_NOOPCODE);
                break;
            }
        }
    }

  error:
    /* TODO: Error handling */
    return;
}

