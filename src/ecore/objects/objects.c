#include "objects.h"

#include "base/object.h"
#include "group/group.h"
#include "misc/key/key.h"
#include "vm/code/code.h"
#include "vm/code/closure.h"


void Eco_Objects_Init()
{
    Eco_Object_Init();
    Eco_Group_Init();
    Eco_Key_Init();
    Eco_Code_Init();
    Eco_Closure_Init();
}

void Eco_Objects_Terminate()
{
    Eco_Object_Terminate();
    Eco_Group_Terminate();
    Eco_Key_Terminate();
    Eco_Code_Terminate();
    Eco_Closure_Terminate();
}
