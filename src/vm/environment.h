#pragma once

#include "../eco.h"


struct Eco_Environment
{
    unsigned int             reference_count;

    struct Eco_Environment*  link;

    unsigned int             var_count;
    Eco_Any*                 vars[];
};
