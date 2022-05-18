#include "mapswitch.h"

#include "replace.h"

void Eco_ReplaceObject(struct Eco*        eco,
                       struct Eco_Object* object,
                       struct Eco_Object* replacement)
{
    /*
     * TODO: Enable the bit sequence 0x02 in the object's gc_bits,
     *       trigger a global GC pointer update, and update all
     *       the maps.
     */
    Eco_GlobalMapUpdate(eco);
}
