#ifndef ECO_VM_MEMORY_MEMORY_H
#define ECO_VM_MEMORY_MEMORY_H

#include <stdlib.h>

struct Eco_Type;


static inline void* Eco_Memory_Alloc(size_t size)
{
    if (size == 0) size = 1;
    return malloc(size);
}

#define Eco_Memory_Free    free

static inline void* Eco_Memory_Realloc(void* ptr, unsigned int new_size)
{
    if (ptr == NULL) {
        return Eco_Memory_Alloc(new_size);
    } else {
        return realloc(ptr, new_size);
    }
}

#endif
