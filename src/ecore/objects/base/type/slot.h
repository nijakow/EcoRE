#ifndef ECO_OBJECTS_BASE_TYPE_SLOT_H
#define ECO_OBJECTS_BASE_TYPE_SLOT_H

#include <ecore/eco.h>

#include "../object/object.h"
#include "slot_info.h"

struct Eco_Molecule;

enum Eco_TypeSlotType
{
    Eco_TypeSlotType_INLINED,
    Eco_TypeSlotType_CODE
};

struct Eco_TypeSlot
{
    enum   Eco_TypeSlotType   type;
    struct Eco_BasicSlotInfo  info;
    struct Eco_Interface*     interface;

    union {
        struct {
            unsigned int      offset;
            unsigned int      value_size;
            struct Eco_Type*  cached_type;
        } inlined;
        struct {
            struct Eco_Code* code;
        } code;
    }                           body;
};

void Eco_TypeSlot_Initialize(struct Eco_TypeSlot*);

bool Eco_TypeSlot_GetValue(struct Eco_TypeSlot*, struct Eco_Molecule*, Eco_Any*);
bool Eco_TypeSlot_SetValue(struct Eco_TypeSlot*, struct Eco_Molecule*, Eco_Any);
bool Eco_TypeSlot_Invoke(struct Eco_Message*, struct Eco_Molecule*, struct Eco_TypeSlot*, Eco_Any);

#endif
