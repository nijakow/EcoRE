#ifndef ECO_CORE_ECO_ECO_H
#define ECO_CORE_ECO_ECO_H

#include "../memory/memory.h"

struct Eco
{
    struct Eco_Heap  heap;
};

static inline Eco_Allocator_t Eco_GetAllocator(struct Eco* self)
{
    return &self->heap;
}

void Eco_Create(struct Eco*);
void Eco_Destroy(struct Eco*);

#endif
