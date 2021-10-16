#include <ecore/eco.h>

#include <ecore/objects/objects.h>
#include <ecore/objects/base/type.h>
#include <ecore/objects/base/object.h>
#include <ecore/objects/group/group.h>

#include <ecore/vm/vm.h>
#include <ecore/vm/builtins/builtins.h>
#include <ecore/vm/memory/gc/gc.h>

#include <ecore/io/econnect/econnect.h>



void Eco_LoadImageFromFile(const char* file)
{
    Eco_VM_LoadImageFromFile(&Eco_THE_VM, file);
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
