#include "pages.h"

static struct Eco_Page* Eco_Pages_AllocateNewPage(struct Eco_Pages* self)
{
    struct Eco_Page*  page;

    page = Eco_Page_New();

    if (page != NULL)
    {
        Eco_Page_Link(page, &self->free_page_list);
    }

    return page;
}

void Eco_Pages_Create(struct Eco_Pages* self)
{
    self->free_page_list =  NULL;
    self->edens[0]       =  NULL;
    self->edens[1]       =  NULL;
    self->current_eden   = &self->edens[0];
}

void Eco_Pages_Destroy(struct Eco_Pages* self)
{
    /*
     * TODO: Free all pages
     */
}
