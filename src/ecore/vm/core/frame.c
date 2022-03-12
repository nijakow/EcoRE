#include "frame.h"

#include <ecore/vm/memory/gc/gc.h>


void Eco_Frame_Mark(struct Eco_GC_State* state, struct Eco_Frame* frame)
{
    unsigned int  i;

    Eco_GC_State_MarkObject(state, (struct Eco_Object*) frame->code);
    
    if (frame->handler != NULL) {
        Eco_GC_State_MarkObject(state, (struct Eco_Closure*) frame->handler);
    }

    for (i = 0; i < frame->code->register_count; i++) {
        Eco_GC_State_MarkAny(state, &(frame->registers[i]));
    }
}
