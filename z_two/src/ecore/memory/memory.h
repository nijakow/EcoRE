#ifndef ECO_MEMORY_MEMORY_H
#define ECO_MEMORY_MEMORY_H

#include <ecore/base/defs.h>

void* eco_memory_alloc(eco_size_t);
void  eco_memory_free(void*);

#endif
