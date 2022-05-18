#include "../map/map.h"

#include "molecule.h"


static unsigned int Eco_Molecule_NormalizeIndex(struct Eco_Map* map, int index)
{
    if (index < 0) return Eco_Map_GetSlotCount(map) + index;
    else           return index;
}


static struct Eco_Object* Eco_Molecule_New(Eco_Allocator_t allocator, struct Eco_Map* map)
{
    struct Eco_Object*  self;

    self = Eco_Object_AllocPlainObject(allocator, map, Eco_MOLECULE_SLOT_COUNT + Eco_Map_GetInstanceSlotCount(map));

    return self;
}


bool Eco_Molecule_AddValueSlot(Eco_Allocator_t      allocator,
                               struct Eco_Object**  self,
                               int                  index,
                               struct Eco_SlotDef   def,
                               Eco_Any              value)
{
    struct Eco_Map*     old_map;
    struct Eco_Map*     new_map;
    struct Eco_Object*  new_self;
    unsigned int        normalized_index;
    unsigned int        i;

    old_map          = Eco_Object_GetMap(*self);
    normalized_index = Eco_Molecule_NormalizeIndex(old_map, index);
    new_map          = Eco_Map_AddValueSlot(old_map, normalized_index, def);

    if (new_map != NULL)
    {
        /*
         * Create a new molecule. This molecule will later replace our
         * previous one if everything goes well.
         */
        new_self = Eco_Molecule_New(allocator, new_map);

        if (new_self != NULL)
        {
            /*
             * Copy the values of the old object into the new one.
             */
            for (i = 0; i < Eco_Map_GetInstanceSlotCount(new_map); i++)
            {
                /*
                 * We only care about inlined slots.
                 */
                if (Eco_MapSlot_IsInlined(Eco_Map_GetSlot(new_map, i)))
                {
                    /*
                     * A three way branch. Decide what to put into the new slot, and
                     * move the data there.
                     */
                    if (i < normalized_index) {
                        Eco_Molecule_Set(new_self,
                                         Eco_Map_GetSlot(new_map, i)->body.inlined.offset,
                                         Eco_Molecule_Get(*self, Eco_Map_GetSlot(old_map, i)->body.inlined.offset));
                    } else if (i == normalized_index) {
                        Eco_Molecule_Set(new_self, Eco_Map_GetSlot(new_map, i)->body.inlined.offset, value);
                    } else {
                        Eco_Molecule_Set(new_self,
                                         Eco_Map_GetSlot(new_map, i)->body.inlined.offset,
                                         Eco_Molecule_Get(*self, Eco_Map_GetSlot(old_map, i - 1)->body.inlined.offset));
                    }
                }
            }

            /*
             * Everything succeeded. Initiate an object switch.
             */
            Eco_Object_Replace(*self, new_self);

            /*
             * Update the original pointer.
             */
            *self = new_self;

            /*
             * Success, return true!
             */
            return true;
        }
    }

    /*
     * Something has failed, return false.
     */
    return false;
}

bool Eco_Molecule_AddCodeSlot(struct Eco_Object*  self,
                              int                 index,
                              struct Eco_SlotDef  def,
                              struct Eco_Object*  code)
{
    struct Eco_Map*  new_map;

    new_map = Eco_Map_AddCodeSlot(self->map, index, def, code);

    /*
     * TODO: - Update the map
     *       - Update the global maps
     */
    return false;
}

bool Eco_Molecule_RemoveSlot(struct Eco_Object* self, int index)
{
    struct Eco_Map*  new_map;

    new_map = Eco_Map_RemoveSlot(self->map, index);

    /*
     * TODO: - Move slots in the original molecule, update the map
     *       - Update the global maps
     */
    return false;
}
