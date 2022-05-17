#include "../base/config.h"

#include <stdlib.h>
#if defined(Eco_CONFIG_ALIGNED_ALLOC_STRATEGY) && Eco_CONFIG_ALIGNED_ALLOC_STRATEGY == Eco_CONFIG_ALIGNED_ALLOC
#  include <malloc.h>
#endif

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
#if defined(Eco_CONFIG_ALIGNED_ALLOC_STRATEGY) && Eco_CONFIG_ALIGNED_ALLOC_STRATEGY == Eco_CONFIG_ALIGNED_ALLOC
    return aligned_alloc(alignment, size);
#else
    void*  ptr;

    /*
     * Linux's posix_memalign is a bit special: If it fails, ptr remains untouched.
     * To avoid errors, we initialize the value to NULL.
     */
    ptr = NULL;
    posix_memalign(&ptr, alignment, size);
    return ptr;
#endif
}

void  Eco_Util_AlignedFree(void* ptr)
{
    free(ptr);
}
