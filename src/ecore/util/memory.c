#include <string.h>

#include "memory.h"


void Eco_Memcpy(void* dst, void* src, unsigned long size)
{
    memcpy(dst, src, size);
}

unsigned int Eco_Strlen(char* str)
{
    return strlen(str);
}
