#include "eco.h"

#include "objects/objects.h"
#include "objects/base/type.h"
#include "objects/base/object.h"
#include "objects/group/group.h"

#include "vm/vm.h"
#include "vm/builtins/builtins.h"
#include "vm/memory/gc/gc.h"

#include "io/econnect/econnect.h"



void Eco_LoadImage(const char* file)
{
    Eco_VM_LoadImage(&Eco_THE_VM, file);
}

void Eco_Run()
{
    Eco_VM_Run(&Eco_THE_VM);
}


void Eco_Init()
{
    Eco_VM_Create(&Eco_THE_VM);
    Eco_Types_Init();
    Eco_Objects_Init();
    Eco_EConnect_Init();
    Eco_VM_Builtins_Init();
}

void Eco_Terminate()
{
    Eco_VM_Builtins_Terminate();
    Eco_EConnect_Terminate();
    Eco_FreeAll();
    Eco_Objects_Terminate();
    Eco_Types_Terminate();
    Eco_VM_Destroy(&Eco_THE_VM);
}
