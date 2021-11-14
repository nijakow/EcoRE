#ifndef ECO_UTIL_UTF8_H
#define ECO_UTIL_UTF8_H

#include <ecore/base/defs.h>

unsigned int Eco_Utf8_Encode(Eco_Codepoint, char*);
bool Eco_Utf8_Decode(const char*, Eco_Codepoint*, unsigned int*);

#endif
