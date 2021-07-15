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
void Eco_EConnect_Instance_OptionallyBindObject(struct Eco_EConnect_Instance*, struct Eco_Object*, unsigned int);
void Eco_EConnect_Instance_UnbindObject(struct Eco_EConnect_Instance*, unsigned int);
struct Eco_Object* Eco_EConnect_Instance_GetBoundObject(struct Eco_EConnect_Instance*, unsigned int);
