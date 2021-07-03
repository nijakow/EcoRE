#pragma once

#include "../../eco.h"


struct Eco_EConnect_State
{
    unsigned int         objects_by_id_max;
    struct Eco_Object**  objects_by_id;
};

void Eco_EConnect_State_Create(struct Eco_EConnect_State*);
void Eco_EConnect_State_Destroy(struct Eco_EConnect_State*);

