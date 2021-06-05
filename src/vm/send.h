#pragma once

#include "../eco.h"

struct Eco_Fiber;


struct Eco_Message
{
    struct Eco_Fiber*   fiber;
    struct Eco_Object*  key;
    struct Eco_Any*     arg_location;
    unsigned int        arg_count;
};


bool Eco_Send(struct Eco_Message*, Eco_Any*);
