#ifndef ECO_CORE_MAP_MAP_H
#define ECO_CORE_MAP_MAP_H

#include "../../base/defs.h"

#include "mapslot.h"


struct Eco_Map
{
    unsigned int        instance_slot_count;
    unsigned int        slot_count;
    struct Eco_MapSlot  slots[];
};

static inline unsigned int Eco_Map_GetInstanceSlotCount(struct Eco_Map* self)
{
    return self->instance_slot_count;
}

static inline struct Eco_MapSlot* Eco_Map_GetSlot(struct Eco_Map* self, unsigned int index)
{
    return &self->slots[index];
}

struct Eco_Map*  Eco_Map_AddValueSlot(struct Eco_Map*, unsigned int, struct Eco_SlotDef, unsigned int*);
struct Eco_Map*  Eco_Map_AddCodeSlot(struct Eco_Map*, unsigned int, struct Eco_SlotDef, struct Eco_Object*);
struct Eco_Map*  Eco_Map_RemoveSlot(struct Eco_Map*, unsigned int);

#endif
