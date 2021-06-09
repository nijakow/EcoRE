#include "closure.h"

#include "type.h"
#include "../vm/memory/memory.h"
#include "../vm/environment.h"


void Eco_Closure_MarkSlots(struct Eco_GC_State* state, struct Eco_Closure* closure)
{
    /* TODO */
}

void Eco_Closure_Del(struct Eco_Closure* closure)
{
    Eco_Environment_Decr(closure->environment);
}


TYPE_DEFINITION(Eco_Closure, NULL, Eco_Closure_MarkSlots, Eco_Closure_Del, 0, {});


struct Eco_Closure* Eco_Closure_New(struct Eco_Code* code, struct Eco_Environment* environment)
{
    struct Eco_Closure* closure;

    closure = Eco_Object_New(NULL, sizeof(struct Eco_Closure));  /* TODO: Type */

    closure->code = code;
    closure->environment = environment;

    Eco_Environment_Incr(environment);

    return closure;
}
