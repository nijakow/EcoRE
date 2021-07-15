#include <stdio.h>

#include "protocol.h"


bool Eco_EConnect_Builtin_Hello(struct Eco_EConnect_Reader* reader,
                                struct Eco_EConnect_Result* result)
{
    printf("Hello :)\n");
    return true;
}
