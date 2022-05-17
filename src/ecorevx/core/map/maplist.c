#include "../../base/defs.h"
#include "../../util/memory.h"

#include "maplist.h"

/*
 * Creates a new MapList.
 */
void Eco_MapList_Create(struct Eco_MapList* self)
{
    self->alloc   = 0;
    self->fill    = 0;
    self->entries = NULL;
}

/*
 * Destroys a MapList.
 */
void Eco_MapList_Destroy(struct Eco_MapList* self)
{
    self->alloc = 0;
    self->fill  = 0;
    if (self != NULL)
        Eco_Util_Free(self);
}

/*
 * Resizes a MapList's entry array.
 */
static void Eco_MapList_Resize(struct Eco_MapList* self)
{
    /*
     * TODO: This function should return an error code
     *       if the reallocation fails!
     */
    struct Eco_Map**  entries;
    unsigned int      new_alloc;

    new_alloc = (self->alloc == 0) ? 4 : (self->alloc * 2);
    entries   = Eco_Util_Realloc(self->entries, new_alloc * sizeof(struct Eco_Map*));

    if (entries != NULL)
    {
        self->entries = entries;
        self->alloc   = new_alloc;
    }
}

/*
 * Inserts an entry into a MapList.
 */
void Eco_MapList_Insert(struct Eco_MapList* self, struct Eco_Map* map)
{
    /*
     * TODO, FIXME, XXX: Handle resize failures!
     */
    if (self->fill >= self->alloc)
        Eco_MapList_Resize(self);
    self->entries[self->fill++] = map;
}

/*
 * Removes an entry from a MapList.
 */
void Eco_MapList_Remove(struct Eco_MapList* self, struct Eco_Map* map)
{
    unsigned int  index;

    for (index = 0; index < self->fill; index++)
    {
        if (self->entries[index] == map)
        {
            self->fill--;

            while (index < self->fill)
            {
                self->entries[index] = self->entries[index + 1];
                index++;
            }

            break;
        }
    }
}
