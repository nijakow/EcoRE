#pragma once

#include "../eco.h"

#include "memory.h"


struct Eco_Environment
{
    unsigned int             reference_count;

    struct Eco_Environment*  link;

    unsigned int             var_count;
    Eco_Any                  vars[];
};


static inline void Eco_Environment_Incr(struct Eco_Environment* environment)
{
    environment->reference_count++;
}

void Eco_Environment_Decr(struct Eco_Environment*);
