#include <string.h>

#include "string.h"


eco_size_t eco_strlen(const char* s)
{
    return strlen(s);
}

bool eco_streq(const char* s1, const char* s2)
{
    return strcmp(s1, s2) == 0;
}

void eco_memcpy(void* dst, const void* src, eco_size_t len)
{
    memcpy(dst, src, len);
}
