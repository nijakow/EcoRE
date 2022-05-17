#ifndef ECO_CORE_MAP_SLOTDEF_H
#define ECO_CORE_MAP_SLOTDEF_H

#include "../any.h"


struct Eco_SlotFlags
{
    unsigned int is_code      : 1;
    unsigned int is_with      : 1;
    unsigned int is_inherited : 1;
    unsigned int is_own       : 1;
};

void Eco_SlotFlags_Create(struct Eco_SlotFlags*);
void Eco_SlotFlags_Destroy(struct Eco_SlotFlags*);


struct Eco_SlotDef
{
    struct Eco_Object*    name;
    struct Eco_SlotFlags  flags;
};

void Eco_SlotDef_Create(struct Eco_SlotDef*, struct Eco_Object*);
void Eco_SlotDef_Destroy(struct Eco_SlotDef*);

#endif
