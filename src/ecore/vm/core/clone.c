#include "clone.h"

#include <ecore/objects/base/object.h>
#include <ecore/objects/base/type.h>
#include <ecore/vm/memory/memory.h>


void Eco_ObjectMap_Create(struct Eco_ObjectMap* self)
{
    self->nodes = NULL;
}

void Eco_ObjectMap_Destroy(struct Eco_ObjectMap* self)
{
    struct Eco_ObjectMapNode*  node;

    while (self->nodes != NULL)
    {
        node        = self->nodes;
        self->nodes = self->nodes->next;
        Eco_Memory_Free(node);
    }
}

void Eco_ObjectMap_Put(struct Eco_ObjectMap* self,
                       struct Eco_Object* key,
                       struct Eco_Object* value)
{
    struct Eco_ObjectMapNode*  node;

    if (self->nodes == NULL || self->nodes->entry_fill >= self->nodes->entry_alloc) {
        node = Eco_Memory_Alloc(sizeof(struct Eco_ObjectMapNode) + sizeof(struct Eco_ObjectMapEntry) * 64);
        node->entry_alloc = 64;
        node->entry_fill  = 0;
        node->next        = self->nodes;
        self->nodes       = node;
    }

    self->nodes->entries[self->nodes->entry_fill].key   = key;
    self->nodes->entries[self->nodes->entry_fill].value = value;
    self->nodes->entry_fill++;
}

struct Eco_Object* Eco_ObjectMap_Get(struct Eco_ObjectMap* self,
                                     struct Eco_Object* key)
{
    struct Eco_ObjectMapNode*  node;
    unsigned int               index;

    for (node = self->nodes; node != NULL; node = node->next)
    {
        for (index = 0; index < node->entry_fill; index++) {
            if (node->entries[index].key == key) {
                return node->entries[index].value;
            }
        }
    }

    return NULL;
}


void Eco_CloneState_Create(struct Eco_CloneState* self)
{
    Eco_ObjectMap_Create(&self->map);
    self->fast.fill = 0;
}

void Eco_CloneState_Destroy(struct Eco_CloneState* self)
{
    Eco_ObjectMap_Destroy(&self->map);
}

struct Eco_Object* Eco_CloneState_CloneObject(struct Eco_CloneState* state, struct Eco_Object* object)
{
    return object->type->typecore->clone(state, object);
}

void Eco_CloneState_CloneAny(struct Eco_CloneState* state, Eco_Any* dst, Eco_Any* src)
{
    if (Eco_Any_IsPointer(src)) {
        Eco_Any_AssignPointer(dst, Eco_CloneState_CloneObject(state, Eco_Any_AsPointer(src)));
    } else {
        Eco_Any_AssignAny(dst, src);
    }
}
