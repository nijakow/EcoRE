#ifndef ECO_CORE_MAP_MAPSLOT_H
#define ECO_CORE_MAP_MAPSLOT_H

#include "slotdef.h"

union Eco_MapSlotBody
{
    struct {
        unsigned int     offset;
        struct Eco_Map*  referenced_map;
    } inlined;

    struct {
    } code;
};

struct Eco_MapSlot
{
    struct Eco_SlotDef      def;
    union  Eco_MapSlotBody  body;
};

#endif
