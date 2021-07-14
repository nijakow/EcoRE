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
