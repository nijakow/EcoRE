#ifndef ECO_UTIL_MEMORY_H
#define ECO_UTIL_MEMORY_H

#include <ecore/base/defs.h>

void Eco_Memcpy(void* dst, const void* src, unsigned long size);

unsigned int Eco_Strlen(const char* str);

bool Eco_StrEq(const char* s1, const char* s2);

#endif
