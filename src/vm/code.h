#pragma once

#include "../eco.h"

struct Eco_Code
{
    u8*           bytecodes;
    
    unsigned int  constant_count;
    Eco_Any*      constants;
};

