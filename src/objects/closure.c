#include "closure.h"

#include "../vm/memory.h"
#include "../vm/environment.h"

struct Eco_Closure* Eco_Closure_New(struct Eco_Code* code, struct Eco_Environment* environment)
{
    struct Eco_Closure* closure;

    closure = Eco_Object_New(NULL, sizeof(struct Eco_Closure));  /* TODO: Type */

    closure->code = code;
    closure->environment = environment;

    Eco_Environment_Incr(environment);

    return closure;
}
