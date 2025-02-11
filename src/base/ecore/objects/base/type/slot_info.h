#ifndef ECO_OBJECTS_BASE_TYPE_SLOT_INFO_H
#define ECO_OBJECTS_BASE_TYPE_SLOT_INFO_H

#include <ecore/objects/misc/key/key.h>

struct Eco_Perspective;

struct Eco_SlotFlags
{
    int is_deprecated : 1;
    int is_protected  : 1;
    int is_static     : 1;
    int is_final      : 1;
    int is_with       : 1;
    int is_inherited  : 1;
    int is_part       : 1;
};

struct Eco_BasicSlotInfo
{
    struct Eco_Key*          key;
    struct Eco_Perspective*  perspective;
    struct Eco_SlotFlags     flags;
};

void Eco_SlotFlags_Create(struct Eco_SlotFlags*);
void Eco_BasicSlotInfo_Create(struct Eco_BasicSlotInfo*);

Eco_Integer Eco_SlotFlags_AsInteger(struct Eco_SlotFlags*);

#endif
