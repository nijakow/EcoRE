#include <stdio.h>

#include "eco.h"


void Eco_Banner()
{
    printf("\n");
    printf("    The Ecosphere\n");
    printf("    Eco Runtime Environment\n");
    printf("\n");
    printf("\n");
}


int main(int argc, char *argv[])
{
    Eco_Banner();
    Eco_Init();
    Eco_Run();
    Eco_Terminate();
    return 0;
}

