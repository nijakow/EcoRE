#ifndef ECO_CORE_MEMORY_HEAP_H
#define ECO_CORE_MEMORY_HEAP_H

#include "page.h"

struct Eco_Heap
{
    struct Eco_Page*   page_list;
    struct Eco_Page*   free_page_list;
    struct Eco_Page*   edens[2];
    struct Eco_Page**  current_eden;
};

void Eco_Heap_Create(struct Eco_Heap*);
void Eco_Heap_Destroy(struct Eco_Heap*);

void* Eco_Heap_AllocateInEden(struct Eco_Heap*, Eco_Size_t);

#endif
