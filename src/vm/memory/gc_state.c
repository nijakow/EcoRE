#include "gc_state.h"

#include "memory.h"


struct Eco_GC_Page
{
    struct Eco_GC_Page*  next;
    
    unsigned int         size;
    unsigned int         fill;

    struct Eco_Object*   elements[];
};



void Eco_GC_State_Create(struct Eco_GC_State* state)
{
    state->pages = NULL;
}

void Eco_GC_State_Destroy(struct Eco_GC_State* state)
{
    /* TODO, XXX */
}


void Eco_GC_State_QueueObject(struct Eco_GC_State* state, struct Eco_Object* object)
{
    struct Eco_GC_Page*  page;

    const unsigned int SIZE = 1024;

    if (state->pages == NULL || state->pages->fill >= state->pages->size) {
        page = Eco_Memory_Alloc(sizeof(struct Eco_GC_Page) + sizeof(struct Eco_Object*) * SIZE);
        page->next        = state->pages;
        page->size        = SIZE;
        page->fill        = 1;
        page->elements[0] = object;
        state->pages      = page;
    } else {
        state->pages->elements[state->pages->fill++] = object;
    }
}


struct Eco_Object* Eco_GC_State_NextObjectToMark(struct Eco_GC_State* state)
{
    struct Eco_Object*   object;
    struct Eco_GC_Page*  page;

    object = state->pages->elements[--(state->pages->fill)];

    if (state->pages->fill == 0) {
        page         = state->pages;
        state->pages = state->pages->next;
        Eco_Memory_Free(page);
    }

    return object;
}
