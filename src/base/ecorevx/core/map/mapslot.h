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
        struct Eco_Object*  code;
    } code;
};


struct Eco_MapSlot
{
    struct Eco_SlotDef      def;
    union  Eco_MapSlotBody  body;
};

static inline bool Eco_MapSlot_IsInlined(struct Eco_MapSlot* self)
{
    return !self->def.flags.is_code;
}

#endif
