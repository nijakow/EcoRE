#pragma once

#include "../../eco.h"

struct Eco_GC_Page;


struct Eco_GC_State
{
    struct Eco_GC_Page*  pages;
};


static inline bool Eco_GC_State_HasObjectsToMark(struct Eco_GC_State* state)
{
    return state->pages != NULL;
}

void Eco_GC_State_QueueObject(struct Eco_GC_State*, struct Eco_Object*);
struct Eco_Object* Eco_GC_State_NextObjectToMark(struct Eco_GC_State*);
