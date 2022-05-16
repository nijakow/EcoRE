#ifndef ECO_UTIL_MEMORY_H
#define ECO_UTIL_MEMORY_H

#include "../base/defs.h"

void* Eco_Util_Alloc(Eco_Size_t);
void* Eco_Util_Realloc(void*, Eco_Size_t);
void  Eco_Util_Free(void*);

void* Eco_Util_AlignedAlloc(Eco_Size_t, Eco_Size_t);
void  Eco_Util_AlignedFree(void*);

#endif
