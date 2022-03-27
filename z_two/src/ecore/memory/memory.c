#include <stdlib.h>

#include "memory.h"

void* eco_memory_alloc(eco_size_t size)
{
    return malloc(size);
}

void  eco_memory_free(void* ptr)
{
    if (ptr != NULL)
        free(ptr);
}
