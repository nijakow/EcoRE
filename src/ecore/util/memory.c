#include <string.h>

#include "memory.h"


void Eco_Memcpy(void* dst, const void* src, unsigned long size)
{
    memcpy(dst, src, size);
}

void Eco_Strncpy(void* dst, const void* src, unsigned long size)
{
    strncpy(dst, src, size);
}

unsigned int Eco_Strlen(const char* str)
{
    return strlen(str);
}

bool Eco_StrEq(const char* s1, const char* s2)
{
    return strcmp(s1, s2) == 0;
}
