#ifndef ECO_VM_CORE_CLONE_H
#define ECO_VM_CORE_CLONE_H

#include <ecore/eco.h>


struct Eco_ObjectMapEntry
{
    struct Eco_Object*  key;
    struct Eco_Object*  value;
};

struct Eco_ObjectMapNode
{
    struct Eco_ObjectMapNode*  next;
    unsigned int               entry_alloc;
    unsigned int               entry_fill;
    struct Eco_ObjectMapEntry  entries[];
};

struct Eco_ObjectMap
{
    struct Eco_ObjectMapNode*  nodes;
};

void Eco_ObjectMap_Create(struct Eco_ObjectMap*);
void Eco_ObjectMap_Destroy(struct Eco_ObjectMap*);
void Eco_ObjectMap_Put(struct Eco_ObjectMap*, struct Eco_Object*, struct Eco_Object*);
struct Eco_Object* Eco_ObjectMap_Get(struct Eco_ObjectMap*, struct Eco_Object*);


#define Eco_CloneState_FAST_ENTRIES 8

struct Eco_CloneState
{
    struct Eco_ObjectMap           map;
    struct {
        unsigned int               fill;
        struct Eco_ObjectMapEntry  entries[Eco_CloneState_FAST_ENTRIES];
    }                              fast;
};

void Eco_CloneState_Create(struct Eco_CloneState*);
void Eco_CloneState_Destroy(struct Eco_CloneState*);
struct Eco_Object* Eco_CloneState_Object(struct Eco_CloneState*, struct Eco_Object*);
void Eco_CloneState_CloneAny(struct Eco_CloneState*, Eco_Any*, Eco_Any*);

static inline struct Eco_Object* Eco_CloneState_QueryClone(struct Eco_CloneState* state,
                                                           struct Eco_Object* object)
{
    unsigned int  i;

    for (i = 0; i < state->fast.fill; i++)
        if (state->fast.entries[i].key == object)
            return state->fast.entries[i].value;
    return Eco_ObjectMap_Get(&state->map, object);
}

static inline void Eco_CloneState_RegisterClone(struct Eco_CloneState* state,
                                                struct Eco_Object* object,
                                                struct Eco_Object* clone)
{
    if (state->fast.fill < Eco_CloneState_FAST_ENTRIES) {
        state->fast.entries[state->fast.fill].key   = object;
        state->fast.entries[state->fast.fill].value = clone;
        state->fast.fill++;
    } else {
        Eco_ObjectMap_Put(&state->map, object, clone);
    }
}

#endif
