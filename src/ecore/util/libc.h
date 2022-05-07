#ifndef ECO_UTIL_LIBC_H
#define ECO_UTIL_LIBC_H

#include <stddef.h>
#include <stdbool.h>

typedef size_t Eco_SizeT;

void* Eco_LibC_Alloc(Eco_SizeT);
void* Eco_LibC_Realloc(void*, Eco_SizeT);
void Eco_LibC_Free(void*);

char* Eco_LibC_Strdup(const char*);

char* Eco_LibC_GetEnv(const char*);

bool Eco_LibC_FileExists(const char*);
bool Eco_LibC_FileIsDirectory(const char*);
bool Eco_LibC_ListFiles(const char*, char**, unsigned int);

#endif

