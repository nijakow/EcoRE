#pragma once

#include <stdarg.h>

enum Eco_Loglevel
{
    Eco_Loglevel_DEBUG    = 0,
    Eco_Loglevel_INFO     = 1,
    Eco_Loglevel_WARNING  = 2,
    Eco_Loglevel_ERROR    = 3,
    Eco_Loglevel_CRITICAL = 4
};

void Eco_Log(enum Eco_Loglevel, const char*, ...);
