#include "frame.h"

#include <ecore/vm/memory/gc/gc.h>


void Eco_Frame_Mark(struct Eco_GC_State* state, struct Eco_Frame* frame)
{
    Eco_GC_State_MarkObject(state, (struct Eco_Object*) frame->code);
    
    if (frame->handler != NULL) {
        Eco_GC_State_MarkObject(state, (struct Eco_Closure*) frame->handler);
    }

    Eco_GC_State_MarkObject(state, frame->code);
    Eco_GC_State_MarkAny(state, frame->myself);

    /* The args and registers will be marked by Eco_Fiber_Mark(...) */
}
