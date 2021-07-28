#include "../fiber.h"

#include "bytecodes.h"
#include "environment.h"
#include "frame.h"
#include "send.h"

#include "../../objects/misc/key/key.h"
#include "../../objects/vm/code/closure.h"

#include "../../io/logging/log.h"


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


bool Eco_Fiber_EnterThunk(struct Eco_Fiber* fiber, Eco_Any* lobby, struct Eco_Code* code)
{
    struct Eco_Frame*  frame;

    frame              = Eco_Fiber_PushFrame(fiber, code->register_count);

    frame->instruction = code->bytecodes;
    frame->code        = code;

    Eco_Any_AssignAny(&frame->self, lobby);

    return true;
}

bool Eco_Fiber_Enter(struct Eco_Fiber*    fiber,
                     struct Eco_Frame*    lexical,
                     struct Eco_Message*  message,
                     struct Eco_Code*     code)
{
    int                i;
    struct Eco_Frame*  frame;

    if (code->arg_count != message->body.send.arg_count) {  /* The +1 stands for the SELF value */
        Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_ARGERROR);
        return false;
    }

    frame = Eco_Fiber_PushFrame(fiber, code->register_count);

    for (i = code->arg_count - 1; i >= 0; i--)
    {
        Eco_Fiber_Pop(fiber, &frame->registers[i]);
    }

    Eco_Fiber_Pop(fiber, &frame->self);

    frame->code        = code;
    frame->instruction = code->bytecodes;

    return true;
}


void Eco_Fiber_Run(struct Eco_Fiber* fiber)
{
    u8                 bytecode;
    struct Eco_Frame*  top;
    struct Eco_Frame*  bottom;

  long_retry:
    top = Eco_Fiber_Top(fiber);
  short_retry:
    if (fiber->state != Eco_Fiber_State_RUNNING)
        goto end;
    while (true)  /* TODO: Execution limit */
    {
        bytecode = Eco_Frame_NextU8(top);
        switch (bytecode)
        {
            case Eco_Bytecode_NOOP: {
                Eco_Log_Debug("-> NOOP\n");
                break;
            }
            case Eco_Bytecode_SELF: {
                u8 reg = Eco_Frame_NextU8(top);
                Eco_Log_Debug("-> SELF %02x\n", reg);
                Eco_Any_AssignAny(&top->registers[reg], &top->self);
                break;
            }
            case Eco_Bytecode_PUSH: {
                u8 reg = Eco_Frame_NextU8(top);
                Eco_Log_Debug("-> PUSH %02x\n", reg);
                Eco_Fiber_Push(fiber, &top->registers[reg]);
                break;
            }
            case Eco_Bytecode_POP: {
                u8 reg = Eco_Frame_NextU8(top);
                Eco_Log_Debug("-> POP %02x\n", reg);
                Eco_Fiber_Pop(fiber, &top->registers[reg]);
                break;
            }
            case Eco_Bytecode_DROP: {
                Eco_Log_Debug("-> DROP\n");
                Eco_Fiber_Drop(fiber);
                break;
            }
            case Eco_Bytecode_DUP: {
                Eco_Log_Debug("-> DUP\n");
                Eco_Fiber_Dup(fiber);
                break;
            }
            case Eco_Bytecode_CONST: {
                u8  to = Eco_Frame_NextU8(top);
                Eco_Log_Debug("-> CONST %02x\n", to);
                Eco_Any_AssignAny(&top->registers[to], Eco_Frame_NextConstant(top));
                break;
            }
            case Eco_Bytecode_R2R: {
                u8 to   = Eco_Frame_NextU8(top);
                u8 from = Eco_Frame_NextU8(top);
                Eco_Log_Debug("-> R2R %02x %02x\n", to, from);
                Eco_Any_AssignAny(&top->registers[to], &top->registers[from]);
                break;
            }
            case Eco_Bytecode_R2L: {
                u8 to    = Eco_Frame_NextU8(top);
                u8 depth = Eco_Frame_NextU8(top);
                u8 from  = Eco_Frame_NextU8(top);
                Eco_Log_Debug("-> R2L %02x:%u %02x\n", to, depth, from);
                bottom   = Eco_Frame_NthLexical(top, depth);
                Eco_Any_AssignAny(&bottom->registers[to], &top->registers[from]);
                break;
            }
            case Eco_Bytecode_L2R: {
                u8 to    = Eco_Frame_NextU8(top);
                u8 from  = Eco_Frame_NextU8(top);
                u8 depth = Eco_Frame_NextU8(top);
                Eco_Log_Debug("-> L2R %02x %02x:%u\n", to, from, depth);
                bottom   = Eco_Frame_NthLexical(top, depth);
                Eco_Any_AssignAny(&top->registers[to], &bottom->registers[from]);
                break;
            }
            case Eco_Bytecode_BUILTIN: {
                u8               args = Eco_Frame_NextU8(top);
                struct Eco_Key*  key  = (struct Eco_Key*) Eco_Any_AsPointer(Eco_Frame_NextConstant(top));   // TODO: Safety check!
                Eco_Log_Debug("-> BUILTIN '%s' %02x\n", key->name, args);
                Eco_Key_CallBuiltin(key, fiber, args);
                goto long_retry;
            }
            case Eco_Bytecode_SEND: {
                struct Eco_Message  message;

                message.body.send.arg_count = Eco_Frame_NextU8(top);
                message.key                 = Eco_Any_AsPointer(Eco_Frame_NextConstant(top));
                message.fiber               = fiber;
                message.type                = Eco_Message_Type_SEND;

                Eco_Log_Debug("-> SEND %u\n", message.body.send.arg_count);

                if (Eco_Send(&message, Eco_Fiber_Nth(fiber, message.body.send.arg_count + 1))) {
                    top = Eco_Fiber_Top(fiber); /* TODO: Do the "slow dispatch" code */
                } else {
                    Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_SENDFAILED);
                    goto end;
                }

                goto long_retry;
            }
            case Eco_Bytecode_ASSIGN: {
                struct Eco_Message  message;

                message.key               = Eco_Any_AsPointer(Eco_Frame_NextConstant(top));
                message.fiber             = fiber;
                message.type              = Eco_Message_Type_ASSIGN;

                Eco_Log_Debug("-> ASSIGN\n");

                Eco_Fiber_Pop(fiber, &message.body.assign.value);

                if (!Eco_Send(&message, Eco_Fiber_Nth(fiber, 1))) {
                    Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_ASSIGNFAILED);
                    goto error;
                }

                goto short_retry;
            }
            case Eco_Bytecode_RETURN: {
                u8                 depth;
                struct Eco_Frame*  target;

                depth = Eco_Frame_NextU8(top);

                Eco_Log_Debug("-> RETURN %u\n", depth);

                while (depth > 0)
                {
                    depth--;
                    target = Eco_Fiber_Top(fiber)->lexical;
                    while (Eco_Fiber_Top(fiber) != target)
                    {
                        Eco_Fiber_PopFrame(fiber);
                        if (!Eco_Fiber_HasTop(fiber)) {
                            Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_RETURNFAILED);
                            goto error;
                        }
                    }
                }

                Eco_Fiber_PopFrame(fiber);

                if (!Eco_Fiber_HasTop(fiber)) {
                    // Last frame was popped, we can now return
                    Eco_Fiber_SetState(fiber, Eco_Fiber_State_TERMINATED);  // TODO: Write a function for this
                    goto end;
                }

                goto long_retry;
            }
            case Eco_Bytecode_MAKE_CLOSURE: {
                u8                   dest;
                u8                   closure_id;
                struct Eco_Closure*  closure;

                dest       = Eco_Frame_NextU8(top);
                closure_id = Eco_Frame_NextU8(top);

                Eco_Log_Debug("-> MAKE_CLOSURE %02x %u\n", dest, closure_id);

                closure    = Eco_Closure_New(top->code->code_instances[closure_id], Eco_Fiber_Top(fiber));

                Eco_Any_AssignPointer(&top->registers[dest], (struct Eco_Object*) closure);

                goto short_retry;
            }
            default: {
                Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_NOOPCODE);
                goto error;
            }
        }
    }

  error:
  end:
    return;
}
