#ifndef ECO_UTIL_MEMORY_H
#define ECO_UTIL_MEMORY_H

#include "../base/defs.h"
#include "../base/config.h"

static inline Eco_Size_t Eco_Util_StandardAlign(Eco_Size_t size)
{
    if ((size % Eco_ALLOCATION_ALIGNMENT) != 0)
        size = (size - (size % Eco_ALLOCATION_ALIGNMENT)) + Eco_ALLOCATION_ALIGNMENT;
    return size;
}

void* Eco_Util_Alloc(Eco_Size_t);
void* Eco_Util_Realloc(void*, Eco_Size_t);
void  Eco_Util_Free(void*);

void* Eco_Util_AlignedAlloc(Eco_Size_t, Eco_Size_t);
void  Eco_Util_AlignedFree(void*);

#endif
