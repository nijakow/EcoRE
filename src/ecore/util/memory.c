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

bool Eco_StrEq(char* s1, char* s2)
{
    return strcmp(s1, s2) == 0;
}
