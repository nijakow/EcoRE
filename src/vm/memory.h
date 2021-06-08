#pragma once

#include <malloc.h>

struct Eco_Type;


#define Eco_Memory_Alloc malloc
#define Eco_Memory_Free  free


void* Eco_Object_New(struct Eco_Type*, unsigned int);
