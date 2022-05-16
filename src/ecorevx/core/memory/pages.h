#ifndef ECO_CORE_MEMORY_PAGES_H
#define ECO_CORE_MEMORY_PAGES_H

#include "page.h"

struct Eco_Pages
{
    struct Eco_Page*  page_list;
};

void Eco_Pages_Create(struct Eco_Pages*);
void Eco_Pages_Destroy(struct Eco_Pages*);

#endif
