#include "float.h"

#include <ecore/objects/base/type.h>
#include <ecore/objects/base/typecore.h>

static struct Eco_TypeCore  Eco_Float_TYPECORE;
       struct Eco_Type*     Eco_Float_TYPE = NULL;


void Eco_Float_Init()
{
    Eco_TypeCore_Create(&Eco_Float_TYPECORE, "Eco_Float");

    Eco_Float_TYPECORE.send = NULL;
    Eco_Float_TYPECORE.mark = NULL;
    Eco_Float_TYPECORE.del  = NULL;

    Eco_Float_TYPE        = Eco_Type_NewPrefab(&Eco_Float_TYPECORE);
}

void Eco_Float_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_Float_TYPECORE);
}
