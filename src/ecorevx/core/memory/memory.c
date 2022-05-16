#include "../../util/memory.h"

#include "memory.h"

static void Eco_PageInfo_Create(struct Eco_PageInfo* self)
{
}

static void Eco_PageInfo_Destroy(struct Eco_PageInfo* self)
{
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
