#pragma once

#include "object.h"

struct Eco_Code;
struct Eco_Environment;


struct Eco_Closure
{
    struct Eco_Object        _;
    struct Eco_Code*         code;
    struct Eco_Environment*  environment;
};
