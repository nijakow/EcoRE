#include "../fiber.h"

#include "../../objects/vm/code/closure.h"

#include "bytecodes.h"
#include "environment.h"
#include "frame.h"
#include "send.h"


struct Eco_Frame* Eco_Fiber_PushFrame(struct Eco_Fiber*        fiber,
                                      unsigned int             register_count)
{
    unsigned int       i;
    struct Eco_Frame*  new_frame;

    new_frame = Eco_Fiber_AllocFrame(fiber, register_count);

    for (i = 0; i < register_count; i++) {
        Eco_Any_AssignInteger(&(new_frame->registers[i]), 0);   /* TODO: Improve that */
    }

    return new_frame;
}

void Eco_Fiber_PopFrame(struct Eco_Fiber* fiber)
{
    struct Eco_Frame*  frame;

    frame = Eco_Fiber_Top(fiber);

    if (frame->delta == 0) {
        fiber->top = NULL;
    } else {
        fiber->stack_alloc_ptr = fiber->stack_alloc_ptr - frame->delta;
        fiber->top             = Eco_Fiber_FrameAt(fiber, fiber->stack_alloc_ptr);
    }
}


bool Eco_Fiber_Enter(struct Eco_Fiber*    fiber,
                     struct Eco_Frame*    lexical,
                     struct Eco_Message*  message,
                     struct Eco_Code*     code)
{
    unsigned int       i;
    struct Eco_Frame*  frame;

    if (code->arg_count != (message->body.send.arg_count + 1)) {  /* The +1 stands for the SELF value */
        Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_ARGERROR);
        return false;
    }

    frame = Eco_Fiber_PushFrame(fiber, code->register_count);

    frame->self        = message->body.send.arg_location[0];

    frame->code        = code;
    frame->instruction = code->bytecodes;

    for (i = 0; i < code->arg_count; i++)
    {
        Eco_Any_AssignAny(&frame->registers[i], &message->body.send.arg_location[i + 1]);
    }

    return true;
}


void Eco_Fiber_Run(struct Eco_Fiber* fiber)
{
    struct Eco_Frame*  top;
    struct Eco_Frame*  bottom;

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
            case Eco_Bytecode_R2L: {
                u8 to    = Eco_Frame_NextU8(top);
                u8 from  = Eco_Frame_NextU8(top);
                u8 depth = Eco_Frame_NextU8(top);
                bottom   = Eco_Frame_NthLexical(top, depth);
                bottom->registers[to] = top->registers[from];
                break;
            }
            case Eco_Bytecode_L2R: {
                u8 to    = Eco_Frame_NextU8(top);
                u8 from  = Eco_Frame_NextU8(top);
                u8 depth = Eco_Frame_NextU8(top);
                bottom   = Eco_Frame_NthLexical(top, depth);
                top->registers[to] = bottom->registers[from];
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
                    goto end;
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
                    goto end;
                }

                break;
            }
            case Eco_Bytecode_RETURN: {
                u8                 depth;
                struct Eco_Frame*  target;

                depth = Eco_Frame_NextU8(top);

                while (depth > 0)
                {
                    target = Eco_Fiber_Top(fiber)->lexical;
                    while (Eco_Fiber_Top(fiber) != target)
                    {
                        Eco_Fiber_PopFrame(fiber);
                        if (!Eco_Fiber_HasTop(fiber)) {
                            Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_RETURNFAILED);
                            goto end;
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
                closure    = Eco_Closure_New(top->code->code_instances[closure_id], Eco_Fiber_Top(fiber));

                Eco_Any_AssignPointer(&top->registers[dest], (struct Eco_Object*) closure);

                break;
            }
            default: {
                Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_NOOPCODE);
                break;
            }
        }
    }

  end:
    return;
}
