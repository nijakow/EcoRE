#pragma once

#include "../eco.h"

struct Eco_Environment;


struct Eco_Frame
{
    unsigned int             delta;

    struct Eco_Environment*  dynamic_vars;

    unsigned int             register_count;
    Eco_Any                  registers[];
};
