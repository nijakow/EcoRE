#include "character.h"

#include <ecore/objects/base/type.h>
#include <ecore/objects/base/typecore.h>

static struct Eco_TypeCore  Eco_Character_TYPECORE;
       struct Eco_Type*     Eco_Character_TYPE = NULL;



void Eco_Character_Init()
{
    Eco_TypeCore_Create(&Eco_Character_TYPECORE, "Eco_Character");

    Eco_Character_TYPECORE.send          = NULL;
    Eco_Character_TYPECORE.mark_instance = NULL;
    Eco_Character_TYPECORE.mark_children = NULL;
    Eco_Character_TYPECORE.del           = NULL;

    Eco_Character_TYPE                   = Eco_Type_NewPrefab(&Eco_Character_TYPECORE);
}

void Eco_Character_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_Character_TYPECORE);
}
