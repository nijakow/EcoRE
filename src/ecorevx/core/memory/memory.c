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
        self->info = Eco_PageInfo(self);
        Eco_PageInfo_Create(self->info);
    }

    return self;
}

void Eco_Page_Delete(struct Eco_Page* self)
{
    Eco_PageInfo_Destroy(self->info);
}
