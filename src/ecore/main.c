#include <stdio.h>

#include "eco.h"

#include "objects/base/object.h"
#include "objects/base/object_slots.h"
#include "objects/base/type.h"
#include "io/econnect/file/file.h"
#include "io/logging/log.h"


void Eco_Banner()
{
    printf("\n");
    printf("    The Ecosphere\n");
    printf("    Eco Runtime Environment\n");
    printf("\n");
}


void Eco_LoadImage(const char* file)
{
    struct Eco_EConnect_Result  result;

    Eco_Log(Eco_Loglevel_INFO, "Loading file '%s'...\n", file);

    if (Eco_EConnect_ReadFile(file, &result)) {
        Eco_Log(Eco_Loglevel_INFO, "Loading file '%s' was successful!\n", file);
    } else {
        Eco_Log(Eco_Loglevel_ERROR, "Can't load file '%s'!\n", file);
    }
}


int main(int argc, char *argv[])
{
    Eco_Banner();
    Eco_Init();

    if (argc != 2) {
        Eco_Log(Eco_Loglevel_ERROR, "Usage: %s <filename.ebf>\n", argv[0]);
    } else {
        Eco_LoadImage(argv[1]);
    }

    Eco_Terminate();
    return 0;
}

