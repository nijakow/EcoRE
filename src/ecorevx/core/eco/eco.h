#ifndef ECO_CORE_ECO_ECO_H
#define ECO_CORE_ECO_ECO_H

#include "../memory/memory.h"

struct Eco
{
    struct Eco_Pages  memory;
};

static inline Eco_Allocator_t Eco_GetAllocator(struct Eco* self)
{
    return &self->memory;
}

#endif
