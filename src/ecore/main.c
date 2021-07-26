#include <stdio.h>

#include "eco.h"

#include "objects/base/object.h"
#include "objects/base/object_slots.h"
#include "objects/base/type.h"
#include "io/logging/log.h"


void Eco_Banner()
{
    printf("\n");
    printf("    The Ecosphere\n");
    printf("    Eco Runtime Environment\n");
    printf("\n");
}


void Eco_Main(int argc, char *argv[])
{
    Eco_Banner();
    Eco_Init();

    if (argc != 2) {
        Eco_Log(Eco_Loglevel_ERROR, "Usage: %s <filename.ebf>\n", argv[0]);
    } else {
        Eco_LoadImage(argv[1]);
        Eco_Run();
    }

    Eco_Terminate();
}


int main(int argc, char *argv[])
{
    Eco_Main(argc, argv);
    return 0;
}

