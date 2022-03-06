#include <string.h>

#include "memory.h"


void Eco_Memcpy(void* dst, void* src, unsigned long size)
{
    memcpy(dst, src, size);
}
