#include "objects.h"

#include "base/object.h"
#include "group/group.h"


void Eco_Objects_Init()
{
    Eco_Object_Init();
    Eco_Group_Init();
}

void Eco_Objects_Terminate()
{
    Eco_Object_Terminate();
    Eco_Group_Terminate();
}
