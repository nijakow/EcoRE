#include "fiber.h"

#include "bytecodes.h"


struct Eco_Frame* Eco_Fiber_PushFrame(struct Eco_Fiber* fiber, unsigned int register_count)
{
    unsigned int      delta;
    struct Eco_Frame* new_frame;

    delta = sizeof(struct Eco_Frame) * Eco_Fiber_Top(fiber)->register_count * sizeof(Eco_Any); 

    fiber->stack_size += delta;

    new_frame = Eco_Fiber_Top(fiber);
    new_frame->delta = delta;
    new_frame->register_count = register_count;

    return new_frame;
}

void Eco_Fiber_PopFrame(struct Eco_Fiber* fiber)
{
    struct Eco_Frame* frame;
    
    frame = Eco_Fiber_Top(fiber);

    if (frame->dynamic_vars != NULL) {
        Eco_Environment_Decr(frame->dynamic_vars);
    }

    fiber->stack_size -= frame->delta;
}


void Eco_Fiber_Run(struct Eco_Fiber* fiber)
{
    struct Eco_Frame* top;

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
                u8 arg_loc = Eco_Frame_NextU8(top);
                u8 arg_cnt = Eco_Frame_NextU8(top);
                /* TODO */
                break;
            }
            default: {
                /* TODO: Error */
            }
        }
    }
}

