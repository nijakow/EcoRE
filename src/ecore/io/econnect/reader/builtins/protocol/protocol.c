#include <stdio.h>

#include "protocol.h"


bool Eco_EConnect_Builtin_Null(struct Eco_EConnect_Reader* reader,
                               struct Eco_EConnect_Result* result)
{
    Eco_EConnect_Result_Create_Object(result, NULL);
    return true;
}


bool Eco_EConnect_Builtin_Hello(struct Eco_EConnect_Reader* reader,
                                struct Eco_EConnect_Result* result)
{
    printf("Hello :)\n");
    return true;
}
