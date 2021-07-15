#include "code.h"

#include "../../base/type.h"
#include "../../../vm/memory/gc/gc.h"


struct Eco_Code* Eco_Code_New()
{
    return Eco_Object_New_Derived(Eco_Type_CODE_TYPE, sizeof(struct Eco_Code), 0);
}

void Eco_Code_Mark(struct Eco_GC_State* state, struct Eco_Code* code)
{
    unsigned int  i;

    for (i = 0; i < code->code_instance_count; i++)
    {
        Eco_Code_Mark(state, code->code_instances[i]);
    }

    for (i = 0; i < code->constant_count; i++)
    {
        Eco_GC_State_MarkAny(state, &(code->constants[i]));
    }

    Eco_Object_Mark(state, &(code->_));
}

void Eco_Code_Del(struct Eco_Code* code)
{
    Eco_Object_Del(&(code->_));
}


/*
 *    T y p e C o r e
 */

struct Eco_TypeCore Eco_Code_TYPECORE;

void Eco_Code_Init()
{
    Eco_TypeCore_Create(&Eco_Code_TYPECORE, "Eco_Code");

    Eco_Code_TYPECORE.send = (Eco_TypeCore_SendFunc) Eco_Object_Send;
    Eco_Code_TYPECORE.mark = (Eco_TypeCore_MarkFunc) Eco_Code_Mark;
    Eco_Code_TYPECORE.del  = (Eco_TypeCore_DelFunc) Eco_Code_Del;
}

void Eco_Code_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_Code_TYPECORE);
}
