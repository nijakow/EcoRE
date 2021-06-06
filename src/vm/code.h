#pragma once

#include "../eco.h"

struct Eco_Code
{
    unsigned int  register_count;
    unsigned int  dynamics_count;

    unsigned int  arg_count;

    u8*           bytecodes;
    
    unsigned int  constant_count;
    Eco_Any*      constants;
};

