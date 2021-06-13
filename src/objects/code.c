#include "code.h"

#include "type.h"


struct Eco_Code* Eco_Code_New()
{
    return Eco_Object_New(Eco_Type_CODE_TYPE, sizeof(struct Eco_Code));
}

void Eco_Code_Mark(struct Eco_GC_State* state, struct Eco_Code* code)
{
    Eco_Object_Mark(state, &(code->_));
}

void Eco_Code_Del(struct Eco_Code* code)
{
    Eco_Object_Del(&(code->_));
}
