#include "../object/object.h"

#include "mapswitch.h"


void Eco_GlobalMapUpdate(struct Eco* eco)
{
    /*
     * TODO
     */
}

void Eco_SwitchMap(struct Eco*        eco,
                   struct Eco_Object* object,
                   struct Eco_Map*    new_map)
{
    object->map = new_map;
    Eco_GlobalMapUpdate(eco);
}
