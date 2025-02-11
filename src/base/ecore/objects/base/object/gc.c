#include <ecore/vm/memory/gc/gc.h>

#include "object.h"
#include "gc.h"


void Eco_Object_MarkChildren(struct Eco_GC_State* state, struct Eco_Object* object)
{
}

void Eco_Object_MarkInstance(struct Eco_GC_State* state, struct Eco_Object* object)
{
    /*
     * It's easy to confuse Eco_Object_Mark(...) and Eco_GC_State_MarkObject(...)!
     * Only call Eco_Object_Mark(...) as the last function in a Eco_<Type>_Mark(...)!
     */
    Eco_GC_State_MarkObject(state, object->type);
}
