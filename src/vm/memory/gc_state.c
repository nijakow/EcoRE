#include "gc_state.h"


struct Eco_GC_Page
{
    struct Eco_GC_Page*  next;
    
    unsigned int         size;
    unsigned int         fill;

    struct Eco_Object*   elements[];
};


void Eco_GC_State_QueueObject(struct Eco_GC_State* state, struct Eco_Object* object)
{
    /* TODO */
}
