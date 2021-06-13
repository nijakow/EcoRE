#include "closure.h"

#include "type.h"
#include "../vm/memory/memory.h"
#include "../vm/environment.h"


struct Eco_Closure* Eco_Closure_New(struct Eco_Code* code, struct Eco_Environment* environment)
{
    struct Eco_Closure* closure;

    closure = Eco_Object_New(Eco_Type_CLOSURE_TYPE, sizeof(struct Eco_Closure));

    closure->code = code;
    closure->environment = environment;

    Eco_Environment_Incr(environment);

    return closure;
}

void Eco_Closure_Mark(struct Eco_GC_State* state, struct Eco_Closure* closure)
{
    /* TODO */
}

void Eco_Closure_Del(struct Eco_Closure* closure)
{
    Eco_Environment_Decr(closure->environment);
}

