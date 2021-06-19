#include "eco.h"

#include "vm/vm.h"


void Eco_Init()
{
    Eco_VM_Create(&Eco_THE_VM);
}

void Eco_Terminate()
{
    Eco_VM_Destroy(&Eco_THE_VM);
}

void Eco_Run()
{
    /* TODO */
}
