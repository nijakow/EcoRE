#pragma once

#include "../../eco.h"


struct Eco_EConnect_Instance
{
    unsigned int         objects_by_id_max;
    struct Eco_Object**  objects_by_id;
};

void Eco_EConnect_Instance_Create(struct Eco_EConnect_Instance*);
void Eco_EConnect_Instance_Destroy(struct Eco_EConnect_Instance*);

void Eco_EConnect_Instance_BindObject(struct Eco_EConnect_Instance*, struct Eco_Object*, unsigned int);
