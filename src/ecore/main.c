#include <stdio.h>

#include "eco.h"

#include "objects/base/object.h"
#include "objects/base/type.h"
#include "io/econnect/file/file.h"


void Eco_Banner()
{
    printf("\n");
    printf("    The Ecosphere\n");
    printf("    Eco Runtime Environment\n");
    printf("\n");
}


void Main_Test1()
{
    struct Eco_EConnect_Result  result;

    Eco_EConnect_ReadFile("../tests/hello.ebf", &result);
    Eco_EConnect_Result_Destroy(&result);
}


void Main_Test2()
{
    struct Eco_Object*  object;
    struct Eco_Object*  key;
    Eco_Any             value;

    object = Eco_Object_New();
    key    = Eco_Object_New();
    Eco_Any_AssignInteger(&value, 42);

    Eco_Object_AddSlot(object, key, 0, &value);
}


int main(int argc, char *argv[])
{
    Eco_Banner();
    Eco_Init();
    // Eco_Run();
    Main_Test1();
    Main_Test2();
    Eco_Terminate();
    return 0;
}

