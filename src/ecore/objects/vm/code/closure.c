#include "closure.h"

#include "code.h"
#include "../../base/type.h"
#include "../../../vm/core/frame.h"
#include "../../../vm/memory/memory.h"


/*
 *    T y p e
 */

static struct Eco_TypeCore Eco_Closure_TYPECORE;
static struct Eco_Type*    Eco_Closure_TYPE;


void Eco_Closure_Init()
{
    Eco_TypeCore_Create(&Eco_Closure_TYPECORE, "Eco_Closure");

    Eco_Closure_TYPECORE.send = (Eco_TypeCore_SendFunc) Eco_Object_Send;
    Eco_Closure_TYPECORE.mark = (Eco_TypeCore_MarkFunc) Eco_Closure_Mark;
    Eco_Closure_TYPECORE.del  = (Eco_TypeCore_DelFunc) Eco_Closure_Del;

    Eco_Closure_TYPE          = Eco_Type_NewPrefab(&Eco_Closure_TYPECORE);
}

void Eco_Closure_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_Closure_TYPECORE);
}



/*
 *    B a s i c s
 */

struct Eco_Closure* Eco_Closure_New(struct Eco_Code* code, struct Eco_Frame* lexical)
{
    struct Eco_Closure* closure;

    closure = Eco_Object_New_Derived(Eco_Closure_TYPE, sizeof(struct Eco_Closure), 0);

    closure->code       = code;
    closure->lexical    = lexical;

    closure->prev       = &(lexical->closures);
    closure->next       = lexical->closures;
    closure->next->prev = &(closure->next);
    lexical->closures   = closure;

    return closure;
}

void Eco_Closure_Mark(struct Eco_GC_State* state, struct Eco_Closure* closure)
{
    Eco_Code_Mark(state, closure->code);
    Eco_Object_Mark(state, &(closure->_));
}

void Eco_Closure_Del(struct Eco_Closure* closure)
{
    if (closure->prev != NULL)
        *(closure->prev) = closure->next;
    if (closure->next != NULL)
        closure->next->prev = NULL;

    Eco_Object_Del(&(closure->_));
}
