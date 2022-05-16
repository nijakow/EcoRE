#include "pages.h"

static struct Eco_Page* Eco_Pages_AllocateNewPageToEden(struct Eco_Pages* self)
{
    struct Eco_Page*  page;

    if (self->free_page_list != NULL) {
        page = self->free_page_list;
        Eco_Page_Unlink(page);
    } else {
        page = Eco_Page_New();
        if (page == NULL)
            return NULL;
    }

    Eco_Page_Link(page, self->current_eden);

    return page;
}

void* Eco_Pages_AllocateInEden(struct Eco_Pages* self, Eco_Size_t size)
{
    struct Eco_Page*  page;
    void*             allocation;

    /*
     * Scan the pages in Eden for a page that has enough space
     * for our allocation.
     * 
     * TODO: Sort pages to speed this up.
     */
    for (page = *self->current_eden;
         page != NULL;
         page = *Eco_Page_NextPage(page))
    {
        allocation = Eco_Page_Alloc(page, size);
        if (allocation != NULL)
            return allocation;
    }

    /*
     * No page has space available!
     * We create a new page, add it to Eden, and retry.
     * 
     * TODO: Note that this happened, adapt the GC accordingly.
     */
    page = Eco_Pages_AllocateNewPageToEden(self);
    if (page == NULL)
        return NULL;
    return Eco_Page_Alloc(page, size);
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
