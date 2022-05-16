#include <stdlib.h>
#include <malloc.h>

#include "memory.h"

void* Eco_Util_Alloc(Eco_Size_t size)
{
    return malloc(size);
}

void* Eco_Util_Realloc(void* ptr, Eco_Size_t size)
{
    if (ptr == NULL)
        return Eco_Util_Alloc(size);
    return realloc(ptr, size);
}

void  Eco_Util_Free(void* ptr)
{
    if (ptr != NULL)
        free(ptr);
}

void* Eco_Util_AlignedAlloc(Eco_Size_t alignment, Eco_Size_t size)
{
    return aligned_alloc(alignment, size);
}

void  Eco_Util_AlignedFree(void* ptr)
{
    free(ptr);
}
