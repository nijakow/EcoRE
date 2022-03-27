#ifndef ECO_UTIL_MEMORY_H
#define ECO_UTIL_MEMORY_H

#include <ecore/base/defs.h>

void Eco_Memcpy(void* dst, void* src, unsigned long size);

unsigned int Eco_Strlen(char* str);

bool Eco_StrEq(char* s1, char* s2);

#endif
