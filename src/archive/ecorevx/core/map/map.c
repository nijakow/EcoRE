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


static struct Eco_Map*  Eco_Map_New(unsigned int slot_count)
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


/*
 * Update the map's inheritance links and invalidation pointers.
 * This function must be called after the core structures of the map have been established.
 */
static void Eco_Map_Finalize(struct Eco_Map* self)
{
    /*
     * TODO
     */
}


static struct Eco_Map*  Eco_Map_CopyWithSlot(struct Eco_Map*      self,
                                             unsigned int         index,
                                             struct Eco_SlotDef   slot_def,
                                             struct Eco_MapSlot** slot_loc)
{
    struct Eco_Map*    copy;
    unsigned int       i;

    /*
     * Create a new map.
     */
    copy = Eco_Map_New(self->slot_count + 1);

    /*
     * Check if the allocation succeeded.
     */
    if (copy != NULL)
    {
        /*
         * Copy the slots over, skip the newly created one.
         */
        for (i = 0; i < copy->slot_count; i++)
        {
                 if (i  < index) copy->slots[i] = self->slots[i];
            else if (i == index);
            else if (i  > index) copy->slots[i] = self->slots[i - 1];
        }

        /*
         * Copy the slot definition from the argument.
         */
        copy->slots[index].def = slot_def;

        /*
         * Set up the indirect return value: A pointer to the slot.
         */
        *slot_loc = &copy->slots[index];
    }

    return copy;
}

static struct Eco_Map*  Eco_Map_CopyWithoutSlot(struct Eco_Map*      self,
                                                unsigned int         index)
{
    struct Eco_Map*    copy;
    unsigned int       i;

    /*
     * Create a new map.
     */
    copy = Eco_Map_New(self->slot_count - 1);

    /*
     * Check if the allocation succeeded.
     */
    if (copy != NULL)
    {
        /*
         * Copy the slots over.
         */
        for (i = 0; i < copy->slot_count; i++)
        {
            copy->slots[i] = self->slots[(i < index) ? i : (i + 1)];
        }
    }

    return copy;
}

struct Eco_Map*  Eco_Map_AddValueSlot(struct Eco_Map*    self,
                                      unsigned int       index,
                                      struct Eco_SlotDef slot_def)
{
    struct Eco_Map*      new_map;
    struct Eco_MapSlot*  new_slot;

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
         * Recompute the slot offsets. This will insert a value into body.inlined.offset
         * and update the map's instance size.
         */
        Eco_Map_RecomputeInlineSlotOffsets(new_map);

        /*
         * Finalize the map. This has to be the last step.
         */
        Eco_Map_Finalize(new_map);
    }

    return new_map;
}

struct Eco_Map*  Eco_Map_AddCodeSlot(struct Eco_Map*    self,
                                     unsigned int       index,
                                     struct Eco_SlotDef slot_def,
                                     struct Eco_Object* code)
{
    struct Eco_Map*      new_map;
    struct Eco_MapSlot*  new_slot;

    new_map = Eco_Map_CopyWithSlot(self, index, slot_def, &new_slot);

    if (new_map != NULL)
    {
        /*
         * Our newly created slot is only rudimentarily initialized. We now
         * set up its code pointer.
         */
        new_slot->body.code.code = code;

        /*
         * Finalize the map. This has to be the last step.
         */
        Eco_Map_Finalize(new_map);
    }

    return new_map;
}

struct Eco_Map*  Eco_Map_RemoveSlot(struct Eco_Map* self, unsigned int index)
{
    struct Eco_Map*      new_map;

    new_map = Eco_Map_CopyWithoutSlot(self, index);

    if (new_map != NULL)
    {
        /*
         * Recompute the slot offsets. This is important as it helps us
         * to save space in the allocated molecules.
         */
        Eco_Map_RecomputeInlineSlotOffsets(new_map);

        /*
         * Finalize the map. This has to be the last step.
         */
        Eco_Map_Finalize(new_map);
    }
    
    return new_map;
}
