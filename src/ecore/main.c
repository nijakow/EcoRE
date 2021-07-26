#include <stdio.h>

#include "eco.h"

#include "objects/base/object.h"
#include "objects/base/object_slots.h"
#include "objects/base/type.h"
#include "io/logging/log.h"


void Eco_Banner()
{
    Eco_Log_Info("\n");
    Eco_Log_Info("    The Ecosphere\n");
    Eco_Log_Info("    Eco Runtime Environment\n");
    Eco_Log_Info("\n");
}


void Eco_Main(int argc, char *argv[])
{
    Eco_Banner();
    Eco_Init();

    if (argc != 2) {
        Eco_Log_Error("Usage: %s <filename.ebf>\n", argv[0]);
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

