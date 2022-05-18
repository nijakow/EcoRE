#include "../map/map.h"

#include "molecule.h"


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
    struct Eco_Map*  new_map;
    unsigned int     value_index;

    new_map = Eco_Map_AddValueSlot((*self)->map, index, def, &value_index);

    /*
     * TODO: - Create a copy of the molecule, move slots, insert value
     *       - Update the object structure (all in one pass)
     *          ^ Update all pointers to the new object
     *          ^ Update the global maps (part of the previous operation:
     *            if the object changes, so does its type)
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
