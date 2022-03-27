#ifndef ECO_UTILS_STRING_H
#define ECO_UTILS_STRING_H

#include <ecore/base/defs.h>

eco_size_t eco_strlen(const char*);

bool eco_streq(const char*, const char*);

void eco_memcpy(void*, const void*, eco_size_t);

#endif
