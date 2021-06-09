#include "memory.h"

#include "../objects/object.h"
#include "../objects/type.h"


struct Eco_Object* OBJECTS = NULL;


void* Eco_Object_New(struct Eco_Type* type, unsigned int size)
{
    struct Eco_Object* object;

    object = Eco_Memory_Alloc(size);

    object->next = OBJECTS;
    object->type = type;

    return object;
}



struct Eco_GC_State
{
};

static inline bool Eco_GC_State_HasObjectsToMark(struct Eco_GC_State* state)
{
    return false;   /* TODO */
}

static inline struct Eco_Object* Eco_GC_State_NextObjectToMark(struct Eco_GC_State* state)
{
    return NULL;    /* TODO */
}


void Eco_GC_MarkRoots(struct Eco_GC_State* state)
{
    /* TODO */
}

void Eco_GC_MarkLoop(struct Eco_GC_State* state)
{
    struct Eco_Object* object;

    while (Eco_GC_State_HasObjectsToMark(state))
    {
        object = Eco_GC_State_NextObjectToMark(state);

        object->header.mark_done = true;
        object->type->shared->mark(state, object);
    }
}

void Eco_GC_Mark(struct Eco_GC_State* state)
{
    Eco_GC_MarkRoots(state);
    Eco_GC_MarkLoop(state);
}

void Eco_GC_Sweep(struct Eco_GC_State* state)
{
    struct Eco_Object*  object;
    struct Eco_Object** ptr;

    ptr = &OBJECTS;

    while (*ptr != NULL)
    {
        object = *ptr;

        if (object->header.mark_done) {
            object->header.mark_queued = false;
            object->header.mark_done   = false;
            ptr = &(object->next);
        } else {
            *ptr = object->next;
            object->type->shared->del(object);
            Eco_Memory_Free(object);
        }
    }
}

void Eco_GC()
{
    struct Eco_GC_State state;

    Eco_GC_Mark(&state);
    Eco_GC_Sweep(&state);
}
