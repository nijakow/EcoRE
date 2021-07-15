#pragma once

#include "../../base/object.h"


struct Eco_Code;
struct Eco_Environment;
struct Eco_GC_State;


struct Eco_Closure
{
    struct Eco_Object        _;

    struct Eco_Code*         code;
    struct Eco_Environment*  environment;
};


struct Eco_Closure* Eco_Closure_New(struct Eco_Code*, struct Eco_Environment*);
void Eco_Closure_Mark(struct Eco_GC_State*, struct Eco_Closure*);
void Eco_Closure_Del(struct Eco_Closure*);

void Eco_Closure_Init();
void Eco_Closure_Terminate();
