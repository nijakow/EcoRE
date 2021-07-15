#include "closure.h"

#include "code.h"
#include "../../base/type.h"
#include "../../../vm/core/environment.h"
#include "../../../vm/memory/memory.h"


struct Eco_Closure* Eco_Closure_New(struct Eco_Code* code, struct Eco_Environment* environment)
{
    struct Eco_Closure* closure;

    closure = Eco_Object_New_Derived(Eco_Type_CLOSURE_TYPE, sizeof(struct Eco_Closure), 0);

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



/*
 *    T y p e C o r e
 */

struct Eco_TypeCore Eco_Closure_TYPECORE;

void Eco_Closure_Init()
{
    Eco_TypeCore_Create(&Eco_Closure_TYPECORE, "Eco_Closure");

    Eco_Closure_TYPECORE.send = (Eco_TypeCore_SendFunc) Eco_Object_Send;
    Eco_Closure_TYPECORE.mark = (Eco_TypeCore_MarkFunc) Eco_Closure_Mark;
    Eco_Closure_TYPECORE.del  = (Eco_TypeCore_DelFunc) Eco_Closure_Del;
}

void Eco_Closure_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_Closure_TYPECORE);
}
