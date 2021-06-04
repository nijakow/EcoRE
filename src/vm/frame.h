#pragma once

#include "../eco.h"

struct Eco_Environment;


struct Eco_Frame
{
    struct Eco_Frame*        previous;

    struct Eco_Environment*  dynamic_vars;

    unsigned int             static_var_count;
    Eco_Any                  static_vars[];
};
