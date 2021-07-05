#include <stdio.h>

#include "eco.h"

#include "io/econnect/file/file.h"


void Eco_Banner()
{
    printf("\n");
    printf("    The Ecosphere\n");
    printf("    Eco Runtime Environment\n");
    printf("\n");
}


void Main_Test()
{
    struct Eco_EConnect_Result  result;

    Eco_EConnect_ReadFile("../tests/hello.ebf", &result);
    Eco_EConnect_Result_Destroy(&result);
}


int main(int argc, char *argv[])
{
    Eco_Banner();
    Eco_Init();
    // Eco_Run();
    Main_Test();
    Eco_Terminate();
    return 0;
}

