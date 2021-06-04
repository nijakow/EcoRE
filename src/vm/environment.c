#include "environment.h"

void Eco_Environment_Decr(struct Eco_Environment* environment)
{
    if (environment == NULL) return;

    if (environment->reference_count <= 1) {
        Eco_Environment_Decr(environment->link);
        Eco_Memory_Free(environment);
    } else {
        environment->reference_count--;
    }
}
