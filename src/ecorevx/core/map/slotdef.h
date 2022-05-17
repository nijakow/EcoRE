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

struct Eco_SlotDef
{
    Eco_Any               name;
    struct Eco_SlotFlags  flags;
};

#endif
