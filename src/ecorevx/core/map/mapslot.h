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

void Eco_MapSlot_CreateInlined(struct Eco_MapSlot*, struct Eco_SlotDef*, unsigned int);
void Eco_MapSlot_CreateCode(struct Eco_MapSlot*, struct Eco_SlotDef*, struct Eco_Object*);
void Eco_MapSlot_Destroy(struct Eco_MapSlot*);

#endif
