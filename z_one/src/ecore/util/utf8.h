#ifndef ECO_UTIL_UTF8_H
#define ECO_UTIL_UTF8_H

#include <ecore/base/defs.h>

#define Eco_Utf8_REPLACEMENT_CHAR ((Eco_Codepoint) 0xfffd)

unsigned int Eco_Utf8_Encode(Eco_Codepoint, char*);
bool Eco_Utf8_Decode(const char*, Eco_Codepoint*, unsigned int*);

#endif
