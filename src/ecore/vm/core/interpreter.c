
#include "bytecodes.h"
#include "frame.h"
#include "send.h"

#include <ecore/vm/fiber/fiber.h>
#include <ecore/vm/fiber/stackops.h>
#include <ecore/objects/misc/key/key.h>
#include <ecore/objects/vm/code/closure.h>


bool Eco_Fiber_EnterThunk(struct Eco_Fiber* fiber, Eco_Any* lobby, struct Eco_Code* code)
{
    struct Eco_Frame*  frame;

    Eco_Fiber_Push(fiber, *lobby);

    frame              = Eco_Fiber_PushFrame(fiber, *lobby, 1, code->arg_count, code->register_count);

    frame->instruction = code->bytecodes;
    frame->code        = code;

    return true;
}

bool Eco_Fiber_Enter(struct Eco_Fiber*  fiber,
                     Eco_Any            myself,
                     struct Eco_Frame*  lexical,
                     struct Eco_Code*   code,
                     unsigned int       arg_count)
{
    struct Eco_Frame*  frame;

    if ((arg_count < code->arg_count)) {
        Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_ARGERROR);
        return false;
    } else if ((arg_count > code->arg_count) && !(code->has_varargs)) {
        while (arg_count > code->arg_count) {
            Eco_Fiber_Pop(fiber);
            arg_count--;
        }
    }

    frame = Eco_Fiber_PushFrame(fiber, myself, arg_count, code->arg_count, code->register_count);

    frame->lexical     = lexical;
    frame->code        = code;
    frame->instruction = code->bytecodes;

    return true;
}

bool Eco_Fiber_EnterClosure(struct Eco_Fiber*   fiber,
                            struct Eco_Closure* closure,
                            unsigned int        args)
{
    /*
     * TODO, FIXME, XXX: What if arg count is zero?
     * TODO: Store SELF in the closure!
     */
    Eco_Any_AssignAny(Eco_Fiber_Nth(fiber, args), &closure->lexical->args[0]);
    return Eco_Fiber_Enter(fiber, closure->lexical->myself, closure->lexical, closure->code, args);
}


static struct Eco_Closure* Eco_Fiber_FindExceptionHandler(struct Eco_Fiber* fiber)
{
    struct Eco_Frame*    frame;
    struct Eco_Closure*  handler;

    frame = Eco_Fiber_Top(fiber);

    while (frame != NULL)
    {
        if (frame->handler != NULL) {
            handler        = frame->handler;
            /*
             * A handler can only be triggered once
             */
            frame->handler = NULL;
            return handler;
        }
        frame = frame->previous;
    }
    return NULL;
}

bool Eco_Fiber_Unwind(struct Eco_Fiber* fiber)
{
    Eco_Any              value;
    struct Eco_Closure*  handler;

    handler = Eco_Fiber_FindExceptionHandler(fiber);
    if (handler != NULL) {
        /*
         * We push a dummy value for Self
         */
        Eco_Fiber_SetRunning(fiber);
        value = Eco_Any_FromInteger(0);
        Eco_Fiber_Push(fiber, value);
        Eco_Fiber_Push(fiber, fiber->thrown);
        Eco_Fiber_EnterClosure(fiber, handler, 2);
        Eco_Fiber_Top(fiber)->return_to = handler->lexical;
        return true;
    }
    return false;
}


static inline u16 CONSTRUCT_U16(u8** ptr) { u16 v = *((u16*) *ptr); *ptr += 2; return v; }

Eco_Any* Eco_Frame_RegisterByBytecode(struct Eco_Frame* frame, u8 address) {
    if ((address & 0x80) != 0)
        return &frame->registers[address & 0x7f];
    else
        return &frame->args[address];
}

void Eco_Fiber_Run(struct Eco_Fiber* fiber, unsigned int steps)
{
    u8*                instruction;
    char*              sp;
    struct Eco_Frame*  top;
    struct Eco_Frame*  bottom;
    unsigned int       instruction_counter;

#include "interpreter_dispatch.h"

    instruction_counter = 0;

  slow_retry:
    top                 = Eco_Fiber_Top(fiber);
    instruction         = top->instruction;
    sp                  = fiber->stack_pointer;
  fast_retry:
    instruction_counter = instruction_counter + 1;
    if (fiber->state != Eco_Fiber_State_RUNNING) {
        switch (fiber->state)
        {
            case Eco_Fiber_State_PAUSED:
            case Eco_Fiber_State_WAITING:
            case Eco_Fiber_State_TERMINATED:
                goto end;
            default:
                goto error;
        }
    }
    if (instruction_counter >= steps)
        goto end;

    DISPATCH(NEXT_U8())
    {
        TARGET(NOOP) {
            ULTRAFAST_DISPATCH();
        }
        TARGET(CONST) {
            u8 to = NEXT_U8();
            Eco_Any_AssignAny(Eco_Frame_RegisterByBytecode(top, to), NEXT_CONSTANT());
            ULTRAFAST_DISPATCH();
        }
        TARGET(PUSHC) {
            FAST_PUSH(NEXT_CONSTANT());
            ULTRAFAST_DISPATCH();
        }
        TARGET(PUSH) {
            u8 reg = NEXT_U8();
            FAST_PUSH(Eco_Frame_RegisterByBytecode(top, reg));
            ULTRAFAST_DISPATCH();
        }
        TARGET(POP) {
            u8 reg = NEXT_U8();
            FAST_POP(Eco_Frame_RegisterByBytecode(top, reg));
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
            Eco_Any_AssignAny(Eco_Frame_RegisterByBytecode(top, to),
                              Eco_Frame_RegisterByBytecode(top, from));
            ULTRAFAST_DISPATCH();
        }
        TARGET(R2L) {
            u8 to    = NEXT_U8();
            u8 depth = NEXT_U8();
            u8 from  = NEXT_U8();
            bottom   = Eco_Frame_NthLexical(top, depth);
            Eco_Any_AssignAny(Eco_Frame_RegisterByBytecode(bottom, to),
                              Eco_Frame_RegisterByBytecode(top, from));
            ULTRAFAST_DISPATCH();
        }
        TARGET(L2R) {
            u8 to    = NEXT_U8();
            u8 from  = NEXT_U8();
            u8 depth = NEXT_U8();
            bottom   = Eco_Frame_NthLexical(top, depth);
            Eco_Any_AssignAny(Eco_Frame_RegisterByBytecode(top, to),
                              Eco_Frame_RegisterByBytecode(bottom, from));
            ULTRAFAST_DISPATCH();
        }
        TARGET(BUILTIN) {
            u8               args = NEXT_U8();
            struct Eco_Key*  key  = Eco_Any_AsPointer(*NEXT_CONSTANT());   // TODO: Safety check!
            top->instruction      = instruction;
            fiber->stack_pointer  = sp;

            if (!Eco_Key_CallBuiltin(key, fiber, args)) {
                Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_BUILTIN_TRAP);
                goto error;
            }
            SLOW_DISPATCH();
        }
        TARGET(BUILTINV) {
            Eco_Any* vararg;

            for (vararg = &top->args[top->named_arg_count]; vararg < top->registers; vararg++) {
                FAST_PUSH(vararg);
            }

            u8               args = NEXT_U8() + Eco_Frame_VarargCount(top);
            struct Eco_Key*  key  = Eco_Any_AsPointer(*NEXT_CONSTANT());   // TODO: Safety check!
            top->instruction      = instruction;
            fiber->stack_pointer  = sp;

            if (!Eco_Key_CallBuiltin(key, fiber, args)) {
                Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_BUILTIN_TRAP);
                goto error;
            }
            SLOW_DISPATCH();
        }
        TARGET(SEND) {
            struct Eco_Message  message;

            message.body.send.arg_count = NEXT_U8();
            message.key                 = Eco_Any_AsPointer(*NEXT_CONSTANT());
            message.fiber               = fiber;
            message.type                = Eco_Message_Type_SEND;

            top->instruction            = instruction;
            fiber->stack_pointer        = sp;

            if (!Eco_Send(&message,
                           *Eco_Fiber_Nth(fiber, message.body.send.arg_count),
                           *Eco_Fiber_Nth(fiber, message.body.send.arg_count),
                           Eco_Any_Equals(Eco_Fiber_Top(fiber)->args[0],
                                         *Eco_Fiber_Nth(fiber, message.body.send.arg_count))
                        || Eco_Any_Equals(Eco_Fiber_Top(fiber)->myself,
                                         *Eco_Fiber_Nth(fiber, message.body.send.arg_count)),
                           false)) {
                Eco_Log_Warning("Message send failed: %s\n", ((struct Eco_Key*) message.key)->name);
                Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_SENDFAILED);
                goto error;
            }
            SLOW_DISPATCH();
        }
        TARGET(SENDV) {
            struct Eco_Message  message;
            Eco_Any*            vararg;

            for (vararg = &top->args[top->named_arg_count]; vararg < top->registers; vararg++) {
                FAST_PUSH(vararg);
            }

            message.body.send.arg_count = NEXT_U8() + Eco_Frame_VarargCount(top);
            message.key                 = Eco_Any_AsPointer(*NEXT_CONSTANT());
            message.fiber               = fiber;
            message.type                = Eco_Message_Type_SEND;

            top->instruction            = instruction;
            fiber->stack_pointer        = sp;

            if (!Eco_Send(&message,
                           *Eco_Fiber_Nth(fiber, message.body.send.arg_count),
                           *Eco_Fiber_Nth(fiber, message.body.send.arg_count),
                           Eco_Any_Equals(Eco_Fiber_Top(fiber)->args[0],
                                         *Eco_Fiber_Nth(fiber, message.body.send.arg_count))
                        || Eco_Any_Equals(Eco_Fiber_Top(fiber)->myself,
                                         *Eco_Fiber_Nth(fiber, message.body.send.arg_count)),
                           false)) {
                Eco_Log_Warning("Message send with varargs failed: %s\n", ((struct Eco_Key*) message.key)->name);
                Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_SENDFAILED);
                goto error;
            }
            SLOW_DISPATCH();
        }
        TARGET(RESEND) {
            struct Eco_Message  message;

            message.body.send.arg_count = NEXT_U8();
            message.key                 = Eco_Any_AsPointer(*NEXT_CONSTANT());
            message.fiber               = fiber;
            message.type                = Eco_Message_Type_SEND;

            top->instruction            = instruction;
            fiber->stack_pointer        = sp;

            if (!Eco_Send(&message,
                           *Eco_Fiber_Nth(fiber, message.body.send.arg_count),
                           Eco_Fiber_Top(fiber)->args[0],
                           true,
                           false)) {
                Eco_Log_Warning("Message send failed: %s\n", ((struct Eco_Key*) message.key)->name);
                Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_SENDFAILED);
                goto error;
            }
            SLOW_DISPATCH();
        }
        TARGET(RESENDV) {
            struct Eco_Message  message;
            Eco_Any*            vararg;

            for (vararg = &top->args[top->named_arg_count]; vararg < top->registers; vararg++) {
                FAST_PUSH(vararg);
            }

            message.body.send.arg_count = NEXT_U8() + Eco_Frame_VarargCount(top);
            message.key                 = Eco_Any_AsPointer(*NEXT_CONSTANT());
            message.fiber               = fiber;
            message.type                = Eco_Message_Type_SEND;

            top->instruction            = instruction;
            fiber->stack_pointer        = sp;

            if (!Eco_Send(&message,
                           *Eco_Fiber_Nth(fiber, message.body.send.arg_count),
                           Eco_Fiber_Top(fiber)->args[0],
                           true,
                           false)) {
                Eco_Log_Warning("Message send with varargs failed: %s\n", ((struct Eco_Key*) message.key)->name);
                Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_SENDFAILED);
                goto error;
            }
            SLOW_DISPATCH();
        }
        TARGET(ASSIGN) {
            struct Eco_Message  message;

            message.key               = Eco_Any_AsPointer(*NEXT_CONSTANT());
            message.fiber             = fiber;
            message.type              = Eco_Message_Type_ASSIGN;

            FAST_POP(&message.body.assign.value);

            top->instruction          = instruction;
            fiber->stack_pointer      = sp;

            if (!Eco_Send(&message,
                           *Eco_Fiber_Nth(fiber, 1),
                           *Eco_Fiber_Nth(fiber, 1),
                           Eco_Any_Equals(Eco_Fiber_Top(fiber)->args[0], *Eco_Fiber_Nth(fiber, 1))
                        || Eco_Any_Equals(Eco_Fiber_Top(fiber)->myself, *Eco_Fiber_Nth(fiber, 1)),
                           false)) {
                Eco_Log_Warning("Assign failed: %s\n", ((struct Eco_Key*) message.key)->name);
                Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_ASSIGNFAILED);
                goto error;
            }

            SLOW_DISPATCH();
        }
        TARGET(AS) {
            struct Eco_Message  message;

            message.key               = Eco_Any_AsPointer(*NEXT_CONSTANT());
            message.fiber             = fiber;
            message.type              = Eco_Message_Type_AS;

            FAST_POP(&message.body.as.value);

            top->instruction          = instruction;
            fiber->stack_pointer      = sp;

            if (!Eco_Send(&message,
                           *Eco_Fiber_Nth(fiber, 1),
                           *Eco_Fiber_Nth(fiber, 1),
                           Eco_Any_Equals(Eco_Fiber_Top(fiber)->args[0],
                                         *Eco_Fiber_Nth(fiber, 1)),
                           false)) {
                Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_ASFAILED);
                goto error;
            }

            SLOW_DISPATCH();
        }
        TARGET(RETURN) {
            u8                 depth;
            struct Eco_Frame*  target;

            depth                 = NEXT_U8();
            fiber->stack_pointer  = sp;

            target = Eco_Frame_NthLexical(Eco_Fiber_Top(fiber), depth)->return_to;

            while (Eco_Fiber_Top(fiber) != target)
            {
                Eco_Fiber_PopFrame(fiber);
            }

            if (!Eco_Fiber_HasTop(fiber)) {
                // Last frame was popped, we can now return
                Eco_Fiber_SetState(fiber, Eco_Fiber_State_TERMINATED);  // TODO: Write a function for this
                goto end;
            }

            SLOW_DISPATCH();
        }
        TARGET(MAKE_CLOSURE) {
            u8                   dest;
            struct Eco_Closure*  closure;

            dest       = NEXT_U8();
            closure    = Eco_Closure_New(Eco_Any_AsPointer(*NEXT_CONSTANT()), Eco_Fiber_Top(fiber));

            *Eco_Frame_RegisterByBytecode(top, dest) = Eco_Any_FromPointer(closure);

            FAST_DISPATCH();
        }
        DEFAULT_TARGET() {
            Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_NOOPCODE);
            goto error;
        }
    }

  error:
    if (Eco_Fiber_Unwind(fiber))
        goto slow_retry;
    else
        return;
  end:
    top->instruction      = instruction;
    fiber->stack_pointer  = sp;
    return;
}
