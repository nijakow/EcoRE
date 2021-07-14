#include "eco.h"

#include "objects/base/type.h"
#include "objects/base/object.h"
#include "objects/group/group.h"

#include "vm/memory/gc/gc.h"

#include "vm/vm.h"

#include "io/econnect/econnect.h"


void Eco_Init()
{
    Eco_VM_Create(&Eco_THE_VM);
    Eco_Type_CreateTypes();
    Eco_EConnect_Init();
}

void Eco_Terminate()
{
    Eco_EConnect_Terminate();
    Eco_FreeAll();
    Eco_Type_DestroyTypes();
    Eco_VM_Destroy(&Eco_THE_VM);
}

void Eco_Run()
{
    Eco_VM_Run(&Eco_THE_VM);
}
