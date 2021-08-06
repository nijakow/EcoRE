#include "frame.h"

#include "../memory/gc/gc.h"

#include "environment.h"


void Eco_Frame_Mark(struct Eco_GC_State* state, struct Eco_Frame* frame)
{
    unsigned int  i;

    Eco_GC_State_MarkObject(state, (struct Eco_Object*) frame->code);

    for (i = 0; i < frame->code->register_count; i++) {
        Eco_GC_State_MarkAny(state, &(frame->registers[i]));
    }
}
