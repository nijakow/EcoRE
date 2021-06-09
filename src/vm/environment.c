#include "environment.h"

#include "memory/memory.h"


struct Eco_Environment* Eco_Environment_New(unsigned int size, struct Eco_Environment* link)
{
    struct Eco_Environment*  environment;

    environment = Eco_Memory_Alloc(sizeof(struct Eco_Environment) + sizeof(Eco_Any) * size);

    if (link != NULL) Eco_Environment_Incr(link);

    environment->reference_count = 1;
    environment->var_count       = size;
    environment->link            = link;

    return environment;
}

void Eco_Environment_Delete(struct Eco_Environment* environment)
{
    Eco_Environment_Decr(environment->link);
    Eco_Memory_Free(environment);
}


void Eco_Environment_Decr(struct Eco_Environment* environment)
{
    if (environment == NULL) return;

    if (environment->reference_count <= 1) {
        Eco_Environment_Delete(environment);
    } else {
        environment->reference_count--;
    }
}
