#pragma once

#include "object.h"

struct Eco_Message;


struct Eco_Group
{
    struct Eco_Object    _;
    struct Eco_Object**  objects;
};


bool Eco_Send_ToGroup(struct Eco_Message*, struct Eco_Group*);
