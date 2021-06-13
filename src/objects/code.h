#pragma once

#include "../eco.h"

#include "object.h"

struct Eco_GC_State;


struct Eco_Code
{
    struct Eco_Object  _;

    unsigned int       register_count;
    unsigned int       dynamics_count;

    unsigned int       arg_count;

    u8*                bytecodes;
    
    unsigned int       constant_count;
    Eco_Any*           constants;

    unsigned int       code_instance_count;
    struct Eco_Code**  code_instances;
};


struct Eco_Code* Eco_Code_New();
void Eco_Code_Mark(struct Eco_GC_State*, struct Eco_Code*);
void Eco_Code_Del(struct Eco_Code*);
