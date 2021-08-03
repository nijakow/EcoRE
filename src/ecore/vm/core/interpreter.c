#include "../fiber.h"

#include "bytecodes.h"
#include "environment.h"
#include "frame.h"
#include "send.h"

#include "../../objects/misc/key/key.h"
#include "../../objects/vm/code/closure.h"

#include "../../io/logging/log.h"


bool Eco_Fiber_EnterThunk(struct Eco_Fiber* fiber, Eco_Any* lobby, struct Eco_Code* code)
{
    struct Eco_Frame*  frame;

    Eco_Fiber_Push(fiber, lobby);

    frame              = Eco_Fiber_AllocFrame(fiber, 1, code->register_count);

    frame->instruction = code->bytecodes;
    frame->code        = code;

    return true;
}

bool Eco_Fiber_Enter(struct Eco_Fiber*  fiber,
                     struct Eco_Frame*  lexical,
                     struct Eco_Code*   code,
                     unsigned int       arg_count)
{
    struct Eco_Frame*  frame;

    if (code->arg_count != arg_count) {
        Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_ARGERROR);
        return false;
    }

    frame = Eco_Fiber_AllocFrame(fiber, arg_count, code->register_count);

    frame->code        = code;
    frame->instruction = code->bytecodes;

    return true;
}


void Eco_Fiber_Run(struct Eco_Fiber* fiber)
{
    u8*                instruction;
    Eco_Any*           registers;
    char*              sp;
    struct Eco_Frame*  top;
    struct Eco_Frame*  bottom;

#include "interpreter_dispatch.h"

  long_retry:
    top         = Eco_Fiber_Top(fiber);
    instruction = top->instruction;
    registers   = top->registers;
    sp          = fiber->stack_pointer;
  short_retry:
    if (fiber->state != Eco_Fiber_State_RUNNING)
        goto end;
    DISPATCH(NEXT_U8())
    {
        TARGET(NOOP) {
            ULTRAFAST_DISPATCH();
        }
        TARGET(CONST) {
            u8 to = NEXT_U8();
            Eco_Any_AssignAny(&registers[to], NEXT_CONSTANT());
            ULTRAFAST_DISPATCH();
        }
        TARGET(PUSHC) {
            FAST_PUSH(NEXT_CONSTANT());
            ULTRAFAST_DISPATCH();
        }
        TARGET(PUSH) {
            u8 reg = NEXT_U8();
            FAST_PUSH(&registers[reg]);
            ULTRAFAST_DISPATCH();
        }
        TARGET(POP) {
            u8 reg = NEXT_U8();
            FAST_POP(&registers[reg]);
            ULTRAFAST_DISPATCH();
        }
        TARGET(DROP) {
            FAST_DROP();
            ULTRAFAST_DISPATCH();
        }
        TARGET(DUP) {
            FAST_DUP();
            ULTRAFAST_DISPATCH();
        }
        TARGET(R2R) {
            u8 to   = NEXT_U8();
            u8 from = NEXT_U8();
            Eco_Any_AssignAny(&registers[to], &registers[from]);
            ULTRAFAST_DISPATCH();
        }
        TARGET(R2L) {
            u8 to    = NEXT_U8();
            u8 depth = NEXT_U8();
            u8 from  = NEXT_U8();
            bottom   = Eco_Frame_NthLexical(top, depth);
            Eco_Any_AssignAny(&bottom->registers[to], &registers[from]);
            ULTRAFAST_DISPATCH();
        }
        TARGET(L2R) {
            u8 to    = NEXT_U8();
            u8 from  = NEXT_U8();
            u8 depth = NEXT_U8();
            bottom   = Eco_Frame_NthLexical(top, depth);
            Eco_Any_AssignAny(&registers[to], &bottom->registers[from]);
            ULTRAFAST_DISPATCH();
        }
        TARGET(BUILTIN) {
            u8               args = NEXT_U8();
            struct Eco_Key*  key  = (struct Eco_Key*) Eco_Any_AsPointer(NEXT_CONSTANT());   // TODO: Safety check!
            top->instruction      = instruction;
            fiber->stack_pointer  = sp;
            Eco_Key_CallBuiltin(key, fiber, args);
            SLOW_DISPATCH();
        }
        TARGET(SEND) {
            struct Eco_Message  message;

            message.body.send.arg_count = NEXT_U8();
            message.key                 = Eco_Any_AsPointer(NEXT_CONSTANT());
            message.fiber               = fiber;
            message.type                = Eco_Message_Type_SEND;

            top->instruction            = instruction;
            fiber->stack_pointer        = sp;

            if (!Eco_Send(&message, Eco_Fiber_Nth(fiber, message.body.send.arg_count))) {
                Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_SENDFAILED);
                goto end;
            }
            SLOW_DISPATCH();
        }
        TARGET(ASSIGN) {
            struct Eco_Message  message;

            message.key               = Eco_Any_AsPointer(NEXT_CONSTANT());
            message.fiber             = fiber;
            message.type              = Eco_Message_Type_ASSIGN;

            Eco_Fiber_Pop(fiber, &message.body.assign.value);

            if (!Eco_Send(&message, Eco_Fiber_Nth(fiber, 1))) {
                Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_ASSIGNFAILED);
                goto error;
            }

            FAST_DISPATCH();
        }
        TARGET(RETURN) {
            u8                 depth;
            struct Eco_Frame*  target;

            depth                 = NEXT_U8();
            fiber->stack_pointer  = sp;

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

            SLOW_DISPATCH();
        }
        TARGET(MAKE_CLOSURE) {
            u8                   dest;
            u8                   closure_id;
            struct Eco_Closure*  closure;

            dest       = NEXT_U8();
            closure_id = NEXT_U8();

            closure    = Eco_Closure_New(top->code->code_instances[closure_id], Eco_Fiber_Top(fiber));

            Eco_Any_AssignPointer(&registers[dest], (struct Eco_Object*) closure);

            FAST_DISPATCH();
        }
        DEFAULT_TARGET() {
            Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_NOOPCODE);
            goto error;
        }
    }

  error:
  end:
    return;
}
