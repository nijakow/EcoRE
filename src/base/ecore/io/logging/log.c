#include <stdio.h>

#include "log.h"


static const char* Eco_LOGLEVELS[] = {
    "[DEBUG]:    ",
    "            ",
    "[WARNING]:  ",
    "[ERROR]:    ",
    "[CRITICAL]: "
};

void Eco_Log(enum Eco_Loglevel level, const char* format, ...)
{
    va_list  args;

    va_start(args, format);
    printf("%s", Eco_LOGLEVELS[level]);
    vprintf(format, args);
    va_end(args);
}
