#include "../../util/memory.h"
#include "heap.h"

#include "page.h"


static void Eco_PageInfo_Create(struct Eco_PageInfo* self)
{
    self->next         = NULL;
    self->prev         = NULL;
    self->next_in_list = NULL;
    self->prev_in_list = NULL;
}

static void Eco_PageInfo_Destroy(struct Eco_PageInfo* self)
{
    /*
     * NOTE: Unlinking the page should not be required since
     *       the page usually gets unlinked before or during
     *       its destruction.
     */
}


struct Eco_Page* Eco_Page_New(struct Eco_Heap* heap)
{
    struct Eco_Page*  self;

    self = Eco_Util_AlignedAlloc(Eco_PAGE_SIZE, Eco_PAGE_SIZE);

    if (self != NULL)
    {
        Eco_Page_Clear(self);
        self->info = Eco_PageInfo(self);
        Eco_PageInfo_Create(self->info);
        *Eco_Page_PrevPage(self) = &heap->page_list;
        *Eco_Page_NextPage(self) =  heap->page_list;
        *Eco_Page_PrevPage(heap->page_list) = Eco_Page_NextPage(self);
    }

    return self;
}

void Eco_Page_Delete(struct Eco_Page* self)
{
    /*
     * Unlink the page from the page list.
     */
    **Eco_Page_PrevPage(self) = *Eco_Page_NextPage(self);
    if (*Eco_Page_NextPage(self) != NULL)
        *Eco_Page_PrevPage(*Eco_Page_NextPage(self)) = *Eco_Page_PrevPage(self);
    
    /*
     * Destroy the PageInfo.
     */
    Eco_PageInfo_Destroy(self->info);

    /*
     * Free the page.
     */
    Eco_Util_AlignedFree(self);
}

/*
 * Allocates data on a page.
 */
void* Eco_Page_Alloc(struct Eco_Page* self, Eco_Size_t size)
{
    void*  ptr;

    /*
     * Align the allocation size.
     */
    size = Eco_Util_StandardAlign(size);

    /*
     * Check if the page has space for this allocation.
     * If the space is exhausted, we fail.
     */
    if (!Eco_Page_HasSpaceFor(self, size))
        return NULL;

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

    next = *Eco_Page_NextPageInList(self);
    prev = *Eco_Page_PrevPageInList(self);

    if (next != NULL)
        *Eco_Page_PrevPageInList(next) = prev;
    if (prev != NULL)
        *prev = next;
    
    *Eco_Page_NextPageInList(self) = NULL;
    *Eco_Page_PrevPageInList(self) = NULL;
}

/*
 * Links the page into a list.
 */
void Eco_Page_Link(struct Eco_Page* self, struct Eco_Page** list)
{
     Eco_Page_Unlink(self);
    *Eco_Page_PrevPageInList(self) =  list;
    *Eco_Page_NextPageInList(self) = *list;
    *list = self;
}
