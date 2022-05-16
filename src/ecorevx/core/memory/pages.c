#include "pages.h"

static struct Eco_Page* Eco_Pages_AllocateNewPage(struct Eco_Pages* self)
{
    struct Eco_Page*  page;

    page = Eco_Page_New();

    if (page != NULL)
    {
        Eco_Page_Link(page, &self->page_list);
    }

    return page;
}

void Eco_Pages_Create(struct Eco_Pages* self)
{
    self->page_list = NULL;
}

void Eco_Pages_Destroy(struct Eco_Pages* self)
{
    /*
     * TODO: Free all pages
     */
}
