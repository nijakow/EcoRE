#include "../../util/memory.h"

#include "page.h"


static void Eco_PageInfo_Create(struct Eco_PageInfo* self)
{
    self->next = NULL;
    self->prev = NULL;
}

static void Eco_PageInfo_Destroy(struct Eco_PageInfo* self)
{
    /*
     * NOTE: Unlinking the page should not be required since
     *       the page usually gets unlinked before or during
     *       its destruction.
     */
}


struct Eco_Page* Eco_Page_New()
{
    struct Eco_Page*  self;

    self = Eco_Util_AlignedAlloc(Eco_PAGE_SIZE, Eco_PAGE_SIZE);

    if (self != NULL)
    {
        Eco_Page_Clear(self);
        self->info = Eco_PageInfo(self);
        Eco_PageInfo_Create(self->info);
    }

    return self;
}

void Eco_Page_Delete(struct Eco_Page* self)
{
    Eco_PageInfo_Destroy(self->info);
}

/*
 * Allocates data on a page.
 */
void* Eco_Page_Alloc(struct Eco_Page* self, Eco_Size_t size)
{
    void*  ptr;

    /*
     * Align the allocation.
     */
    size        = Eco_Util_StandardAlign(size);

    /*
     * The classic pointer increment :)
     */
    ptr         = Eco_Page_Here(self);
    self->alloc = self->alloc + size;

    return ptr;
}

/*
 * Clears the page by resetting its allocation pointer.
 */
void Eco_Page_Clear(struct Eco_Page* self)
{
    self->alloc = Eco_Page_GetHeapStart(self);
}


/*
 * Unlinks the page from its current list.
 */
void Eco_Page_Unlink(struct Eco_Page* self)
{
    struct Eco_Page*   next;
    struct Eco_Page**  prev;

    next = *Eco_Page_NextPage(self);
    prev = *Eco_Page_PrevPage(self);

    if (next != NULL)
        *Eco_Page_PrevPage(next) = prev;
    if (prev != NULL)
        *prev = next;
    
    *Eco_Page_NextPage(self) = NULL;
    *Eco_Page_PrevPage(self) = NULL;
}

/*
 * Links the page into a list.
 */
void Eco_Page_Link(struct Eco_Page* self, struct Eco_Page** list)
{
     Eco_Page_Unlink(self);
    *Eco_Page_PrevPage(self) =  list;
    *Eco_Page_NextPage(self) = *list;
    *list = self;
}
