#ifndef ECO_VM_CORE_ENVIRONMENT_H
#define ECO_VM_CORE_ENVIRONMENT_H

#include "../../eco.h"

#include "../memory/memory.h"
#include "../memory/gc/gc.h"


struct Eco_Environment
{
    unsigned int             reference_count;

    struct Eco_Environment*  link;

    unsigned int             var_count;
    Eco_Any                  vars[];
};


struct Eco_Environment* Eco_Environment_New(unsigned int, struct Eco_Environment*);


static inline void Eco_Environment_Incr(struct Eco_Environment* environment)
{
    environment->reference_count++;
}

void Eco_Environment_Decr(struct Eco_Environment*);

void Eco_Environment_Mark(struct Eco_GC_State*, struct Eco_Environment*);

#endif
