#include "clone.h"

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
