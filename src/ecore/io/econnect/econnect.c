#include "econnect.h"

#include "reader/builtins/builtins.h"


void Eco_EConnect_Init()
{
    Eco_EConnect_InitReaderBuiltins();
}

void Eco_EConnect_Terminate()
{
    Eco_EConnect_TerminateReaderBuiltins();
}
