#include "gc.h"

#include "../memory.h"
#include "gc_state.h"

#include "../../../objects/object.h"
#include "../../../objects/type.h"

#include "../../vm.h"


extern struct Eco_Object* Eco_OBJECTS;


void Eco_GC_MakeSticky(struct Eco_Object* object)
{
    /* TODO, FIXME, XXX: IMPLEMENT THIS! */
}

void Eco_GC_MarkRoots(struct Eco_GC_State* state)
{
    Eco_GC_State_MarkObject(state, Eco_Type_TYPE_TYPE);
    Eco_GC_State_MarkObject(state, Eco_Type_GROUP_TYPE);
    Eco_GC_State_MarkObject(state, Eco_Type_CODE_TYPE);
    Eco_GC_State_MarkObject(state, Eco_Type_CLOSURE_TYPE);

    Eco_VM_Mark(state, &Eco_THE_VM);
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

    ptr = &Eco_OBJECTS;

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
        }
    }
}

void Eco_GC()
{
    struct Eco_GC_State  state;

    Eco_GC_State_Create(&state);
    Eco_GC_Mark(&state);
    Eco_GC_Sweep(&state);
    Eco_GC_State_Destroy(&state);
}

void Eco_FreeAll()
{
    struct Eco_GC_State  state;

    Eco_GC_State_Create(&state);
    Eco_GC_Sweep(&state);
    Eco_GC_State_Destroy(&state);
}
