#include "eco.h"

#include "objects/type.h"
#include "objects/object.h"
#include "objects/group.h"

#include "vm/memory/gc.h"

#include "vm/vm.h"


void Eco_Init()
{
    Eco_VM_Create(&Eco_THE_VM);
    Eco_Type_InitializeTypes();
}

void Eco_Terminate()
{
    Eco_FreeAll();
    Eco_VM_Destroy(&Eco_THE_VM);
}

void Eco_Run()
{
    Eco_VM_Run(&Eco_THE_VM);
}
