#ifndef ECO_IO_LOGGING_LOG_H
#define ECO_IO_LOGGING_LOG_H

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

#define Eco_Log_Debug(...)    Eco_Log(Eco_Loglevel_DEBUG, __VA_ARGS__)
#define Eco_Log_Info(...)     Eco_Log(Eco_Loglevel_INFO, __VA_ARGS__)
#define Eco_Log_Warning(...)  Eco_Log(Eco_Loglevel_WARNING, __VA_ARGS__)
#define Eco_Log_Error(...)    Eco_Log(Eco_Loglevel_ERROR, __VA_ARGS__)
#define Eco_Log_Critical(...) Eco_Log(Eco_Loglevel_CRITICAL, __VA_ARGS__)

#endif
