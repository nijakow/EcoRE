#ifndef ECO_VM_MEMORY_GC_GC_STATE_H
#define ECO_VM_MEMORY_GC_GC_STATE_H

#include <ecore/eco.h>

struct Eco_GC_Page;
struct Eco_VM;


struct Eco_GC_State
{
    struct Eco_GC_Page*  pages;
    struct Eco_VM*       vm;
};


void Eco_GC_State_Create(struct Eco_GC_State*, struct Eco_VM* vm);
void Eco_GC_State_Destroy(struct Eco_GC_State*);

static inline bool Eco_GC_State_HasObjectsToMark(struct Eco_GC_State* state)
{
    return state->pages != NULL;
}

void Eco_GC_State_QueueObject(struct Eco_GC_State*, struct Eco_Object*);
struct Eco_Object* Eco_GC_State_NextObjectToMark(struct Eco_GC_State*);

#endif
