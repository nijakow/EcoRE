#include "closure.h"

#include "type.h"
#include "code.h"
#include "../vm/core/environment.h"
#include "../vm/memory/memory.h"


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
    Eco_Code_Mark(state, closure->code);
    Eco_Environment_Mark(state, closure->environment);
    Eco_Object_Mark(state, &(closure->_));
}

void Eco_Closure_Del(struct Eco_Closure* closure)
{
    Eco_Environment_Decr(closure->environment);
    Eco_Object_Del(&(closure->_));
}

