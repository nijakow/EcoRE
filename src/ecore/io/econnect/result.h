#pragma once

#include "../../eco.h"


enum Eco_EConnect_Result_Type
{
    Eco_EConnect_Result_Type_NONE,
    Eco_EConnect_Result_Type_ERROR,
    Eco_EConnect_Result_Type_OBJECT,
    Eco_EConnect_Result_Type_ANY
};

struct Eco_EConnect_Result
{
    enum Eco_EConnect_Result_Type  type;

    union {
        Eco_Any  any;
    } body;
};

void Eco_EConnect_Result_Destroy(struct Eco_EConnect_Result*);
