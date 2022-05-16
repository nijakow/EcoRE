#include <string.h>

#include "libc.h"

Eco_Size_t Eco_Util_Strlen(const char* str)
{
    return strlen(str);
}

void Eco_Util_Strcpy(char* dst, const char* src)
{
    strcpy(dst, src);
}
