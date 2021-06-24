#pragma once

#include "../eco.h"

#include "object.h"


struct Eco_EConnect_Message;


struct Eco_Key
{
    struct Eco_Object  _;

    struct Eco_Key*    next;
    struct Eco_Key*    prev;

    char*              name;

    void* (*econnect_func)(struct Eco_EConnect_Message*);
};

struct Eco_Key* Eco_Key_Find(const char*);

void Eco_Key_Mark(struct Eco_GC_State*, struct Eco_Key*);
void Eco_Key_Del(struct Eco_Key*);
