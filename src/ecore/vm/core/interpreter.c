#include "../fiber.h"

#include "bytecodes.h"
#include "frame.h"
#include "send.h"

#include <ecore/objects/misc/key/key.h>
#include <ecore/objects/vm/code/closure.h>


bool Eco_Fiber_EnterThunk(struct Eco_Fiber* fiber, Eco_Any* lobby, struct Eco_Code* code)
{
    struct Eco_Frame*  frame;

    Eco_Fiber_Push(fiber, lobby);

    frame              = Eco_Fiber_PushFrame(fiber, 1, code->arg_count, code->register_count);

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

    if ((arg_count < code->arg_count) || ((arg_count > code->arg_count) && !(code->has_varargs))) {
        Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_ARGERROR);
        return false;
    }

    frame = Eco_Fiber_PushFrame(fiber, arg_count, code->arg_count, code->register_count);

    frame->lexical     = lexical;
    frame->code        = code;
    frame->instruction = code->bytecodes;

    return true;
}


inline u16 CONSTRUCT_U16(u8** ptr) { u16 v = *((u16*) *ptr); *ptr += 2; return v; }

void Eco_Fiber_Run(struct Eco_Fiber* fiber, unsigned int steps)
{
    u8*                instruction;
    Eco_Any*           registers;
    char*              sp;
    struct Eco_Frame*  top;
    struct Eco_Frame*  bottom;
    unsigned int       instruction_counter;

#include "interpreter_dispatch.h"

    instruction_counter = 0;

  slow_retry:
    top                 = Eco_Fiber_Top(fiber);
    instruction         = top->instruction;
    registers           = top->registers;
    sp                  = fiber->stack_pointer;
  fast_retry:
    instruction_counter = instruction_counter + 1;
    if (instruction_counter >= steps)
        goto end;
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
        TARGET(BUILTINV) {
            unsigned int  i;

            for (i = 0; i < top->vararg_count; i++) {
                FAST_PUSH(&top->varargs[i]);
            }

            u8               args = NEXT_U8() + i;
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

            if (!Eco_Send(&message, NULL, Eco_Fiber_Nth(fiber, message.body.send.arg_count))) {
                Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_SENDFAILED);
                goto end;
            }
            SLOW_DISPATCH();
        }
        TARGET(SENDV) {
            unsigned int        i;
            struct Eco_Message  message;

            for (i = 0; i < top->vararg_count; i++) {
                FAST_PUSH(&top->varargs[i]);
            }

            message.body.send.arg_count = NEXT_U8() + i;
            message.key                 = Eco_Any_AsPointer(NEXT_CONSTANT());
            message.fiber               = fiber;
            message.type                = Eco_Message_Type_SEND;

            top->instruction            = instruction;
            fiber->stack_pointer        = sp;

            if (!Eco_Send(&message, NULL, Eco_Fiber_Nth(fiber, message.body.send.arg_count))) {
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

            FAST_POP(&message.body.assign.value);

            top->instruction          = instruction;
            fiber->stack_pointer      = sp;

            if (!Eco_Send(&message, NULL, Eco_Fiber_Nth(fiber, 1))) {
                Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_ASSIGNFAILED);
                goto error;
            }

            SLOW_DISPATCH();
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
            u16                  closure_id;
            struct Eco_Closure*  closure;

            dest       = NEXT_U8();
            closure_id = NEXT_U16();

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
    top->instruction      = instruction;
    fiber->stack_pointer  = sp;
    return;
}
