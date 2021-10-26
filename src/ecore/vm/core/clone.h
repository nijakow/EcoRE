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


struct Eco_CloneState
{
    struct Eco_ObjectMap  map;
};


#endif
