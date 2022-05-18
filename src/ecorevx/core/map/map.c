#include "../../base/defs.h"
#include "../../util/memory.h"

#include "map.h"

static inline unsigned int Eco_Map_NormalizeIndex(struct Eco_Map* self, int index)
{
    if (index < 0) return self->slot_count + index;
    else           return index;
}

struct Eco_Map*  Eco_Map_New(unsigned int slot_count)
{
    struct Eco_Map*  self;

    self = Eco_Util_Alloc(sizeof(struct Eco_Map) + slot_count * sizeof(struct Eco_MapSlot));

    if (self != NULL)
    {
        /*
         * TODO: Initialize the basic slots
         */
        self->slot_count = slot_count;
    }

    return self;
}

struct Eco_Map*  Eco_Map_CopyWithSlot(struct Eco_Map*      self,
                                      int                  index,
                                      struct Eco_SlotDef   slot_def,
                                      struct Eco_MapSlot** slot_loc)
{
    const unsigned int slot_count = self->slot_count + 1;
    const unsigned int slot_index = Eco_Map_NormalizeIndex(self, index);
    struct Eco_Map*    copy;
    unsigned int       i;

    copy = Eco_Map_New(slot_count);

    if (copy != NULL)
    {
        copy->instance_slot_count = self->instance_slot_count;
        copy->slot_count          = self->slot_count;
        for (i = 0; i < copy->slot_count; i++)
        {
            copy->slots[i] = self->slots[(i < slot_index) ? i : (i + 1)];
        }
        /*
         * TODO: Update the map pointers inside of the slots!
         */
    }

    return copy;
}

struct Eco_Map*  Eco_Map_AddValueSlot(struct Eco_Map*    self,
                                      int                index,
                                      struct Eco_SlotDef slot_def,
                                      unsigned int*      value_index_loc)
{
    /*
     * TODO
     */
    return NULL;
}

struct Eco_Map*  Eco_Map_AddCodeSlot(struct Eco_Map*    self,
                                     int                index,
                                     struct Eco_SlotDef slot_def,
                                     struct Eco_Object* code)
{
    /*
     * TODO
     */
    return NULL;
}

struct Eco_Map*  Eco_Map_RemoveSlot(struct Eco_Map* self, int index)
{
    /*
     * TODO
     */
    return NULL;
}
