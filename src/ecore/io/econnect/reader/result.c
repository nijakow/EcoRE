#include "result.h"


void Eco_EConnect_Result_Create_None(struct Eco_EConnect_Result* result)
{
    result->type = Eco_EConnect_Result_Type_NONE;
}

void Eco_EConnect_Result_Create_Error(struct Eco_EConnect_Result* result,
                                      enum Eco_EConnect_Result_ErrorType error)
{
    result->type       = Eco_EConnect_Result_Type_ERROR;
    result->body.error = error;
}

void Eco_EConnect_Result_Create_Any(struct Eco_EConnect_Result* result, Eco_Any* value)
{
    result->type = Eco_EConnect_Result_Type_ANY;
    Eco_Any_AssignAny(&(result->body.any), value);
}

void Eco_EConnect_Result_Create_Object(struct Eco_EConnect_Result* result, struct Eco_Object* value)
{
    result->type = Eco_EConnect_Result_Type_ANY;
    result->body.any = Eco_Any_FromPointer(value);
}

void Eco_EConnect_Result_Create_Integer(struct Eco_EConnect_Result* result, Eco_Integer value)
{
    result->type = Eco_EConnect_Result_Type_ANY;
    result->body.any = Eco_Any_FromInteger(value);
}

void Eco_EConnect_Result_Create_Character(struct Eco_EConnect_Result* result, Eco_Codepoint value)
{
    result->type = Eco_EConnect_Result_Type_ANY;
    result->body.any = Eco_Any_FromCharacter(value);
}

void Eco_EConnect_Result_Copy(struct Eco_EConnect_Result* dst, struct Eco_EConnect_Result* src)
{
    *dst = *src;
}

void Eco_EConnect_Result_Destroy(struct Eco_EConnect_Result* result)
{
    switch (result->type) {
        default: {
        }
    }
}


bool Eco_EConnect_Result_ExpectAny(struct Eco_EConnect_Result* result,
                                   Eco_Any* any_loc)
{
    if (result->type == Eco_EConnect_Result_Type_ANY) {
        *any_loc = result->body.any;
        return true;
    } else {
        return false;
    }
}

bool Eco_EConnect_Result_ExpectObject(struct Eco_EConnect_Result* result,
                                      struct Eco_Object** obj_loc)
{
    if (result->type == Eco_EConnect_Result_Type_ANY) {
        *obj_loc = Eco_Any_AsPointer(result->body.any);
        return true;
    } else {
        *obj_loc = NULL;
        return false;
    }
}
