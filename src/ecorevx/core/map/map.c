#include "../../base/defs.h"
#include "../../util/memory.h"

#include "map.h"


static void Eco_Map_RecomputeInlineSlotOffsets(struct Eco_Map* self)
{
    unsigned int i;
    unsigned int j;

    j = 0;
    for (i = 0; i < Eco_Map_GetInstanceSlotCount(self); i++)
    {
        if (Eco_MapSlot_IsInlined(Eco_Map_GetSlot(self, i)))
        {
            Eco_Map_GetSlot(self, i)->body.inlined.offset = j++;
        }
    }
    self->instance_slot_count = j;
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
                                      unsigned int         index,
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
        copy->slots[index].def = slot_def;
        /*
         * TODO: Update the map pointers inside of the slots!
         */
    }

    return copy;
}

struct Eco_Map*  Eco_Map_AddValueSlot(struct Eco_Map*    self,
                                      unsigned int       index,
                                      struct Eco_SlotDef slot_def,
                                      unsigned int*      value_index_loc)
{
    struct Eco_Map*      new_map;
    struct Eco_MapSlot*  new_slot;
    unsigned int         i;
    unsigned int         j;

    new_map = Eco_Map_CopyWithSlot(self, index, slot_def, &new_slot);

    if (new_map != NULL)
    {
        /*
         * Our newly created slot is only rudimentarily initialized. We now
         * set up its referenced_map pointer. If the slot is a "with" slot,
         * the pointer should be initialized with a pointer to its own map.
         */
        new_slot->body.inlined.referenced_map = (slot_def.flags.is_with) ? new_map : NULL;

        /*
         * 
         */
        Eco_Map_RecomputeInlineSlotOffsets(new_map);
    }

    return new_map;
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
