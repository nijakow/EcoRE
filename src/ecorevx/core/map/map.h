#ifndef ECO_CORE_MAP_MAP_H
#define ECO_CORE_MAP_MAP_H

#include "mapslot.h"

struct Eco_Map
{
    unsigned int        slot_count;
    struct Eco_MapSlot  slots[];
};

#endif
