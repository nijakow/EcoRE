#pragma once

#include "../../eco.h"

struct Eco_GC_Page;


struct Eco_GC_State
{
    struct Eco_GC_Page*  pages;
};


static inline bool Eco_GC_State_HasObjectsToMark(struct Eco_GC_State* state)
{
    return false;   /* TODO */
}

static inline struct Eco_Object* Eco_GC_State_NextObjectToMark(struct Eco_GC_State* state)
{
    return NULL;    /* TODO */
}


void Eco_GC_State_QueueObject(struct Eco_GC_State*, struct Eco_Object*);
