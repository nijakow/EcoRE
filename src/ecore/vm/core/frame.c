#include "frame.h"

#include "../memory/gc/gc.h"

#include "environment.h"


void Eco_Frame_Mark(struct Eco_GC_State* state, struct Eco_Frame* frame)
{
    unsigned int  i;

    Eco_GC_State_MarkAny(state, &(frame->self));
    Eco_GC_State_MarkObject(state, (struct Eco_Object*) frame->code);
    Eco_Environment_Mark(state, frame->dynamic_vars);

    for (i = 0; i < frame->register_count; i++) {
        Eco_GC_State_MarkAny(state, &(frame->registers[i]));
    }
}
