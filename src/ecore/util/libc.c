#include <stdlib.h>

#include "libc.h"

char* Eco_LibC_GetEnv(const char* varname)
{
    return getenv(varname);
}

