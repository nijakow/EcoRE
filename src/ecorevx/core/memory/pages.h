#ifndef ECO_CORE_MEMORY_PAGES_H
#define ECO_CORE_MEMORY_PAGES_H

#include "page.h"

struct Eco_Pages
{
    struct Eco_Page*   free_page_list;
    struct Eco_Page*   edens[2];
    struct Eco_Page**  current_eden;
};

void Eco_Pages_Create(struct Eco_Pages*);
void Eco_Pages_Destroy(struct Eco_Pages*);

#endif
