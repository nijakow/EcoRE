#include "integer.h"

#include <ecore/objects/base/type.h>
#include <ecore/objects/base/typecore.h>

static struct Eco_TypeCore  Eco_Integer_TYPECORE;
       struct Eco_Type*     Eco_Integer_TYPE = NULL;



void Eco_Integer_Init()
{
    Eco_TypeCore_Create(&Eco_Integer_TYPECORE, "Eco_Integer");

    Eco_Integer_TYPECORE.send          = NULL;
    Eco_Integer_TYPECORE.mark_instance = NULL;
    Eco_Integer_TYPECORE.mark_children = NULL;
    Eco_Integer_TYPECORE.del           = NULL;

    Eco_Integer_TYPE          = Eco_Type_NewPrefab(&Eco_Integer_TYPECORE);
}

void Eco_Integer_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_Integer_TYPECORE);
}
