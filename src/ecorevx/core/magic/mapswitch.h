#ifndef ECO_CORE_MAGIC_MAPSWITCH_H
#define ECO_CORE_MAGIC_MAPSWITCH_H

#include "../../base/defs.h"
#include "../eco/eco.h"

void Eco_GlobalMapUpdate(struct Eco*);
void Eco_SwitchMap(struct Eco*, struct Eco_Object*, struct Eco_Map*);

#endif
