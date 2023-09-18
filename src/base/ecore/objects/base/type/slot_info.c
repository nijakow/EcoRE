#include <ecore/base/defs.h>

#include "slot_info.h"


void Eco_SlotFlags_Create(struct Eco_SlotFlags* flags)
{
    flags->is_deprecated = false;
    flags->is_protected  = false;
    flags->is_static     = false;
    flags->is_final      = false;
    flags->is_inherited  = false;
    flags->is_with       = false;
    flags->is_part       = false;
}

void Eco_BasicSlotInfo_Create(struct Eco_BasicSlotInfo* info)
{
    info->key         = NULL;
    info->perspective = NULL;
    Eco_SlotFlags_Create(&info->flags);
}

Eco_Integer Eco_SlotFlags_AsInteger(struct Eco_SlotFlags* flags) {
    Eco_Integer  value;

    value = 0;

    if (flags->is_deprecated) value |= 0x01;
    if (flags->is_protected)  value |= 0x02;
    if (flags->is_static)     value |= 0x04;
    if (flags->is_final)      value |= 0x08;
    if (flags->is_with)       value |= 0x10;
    if (flags->is_inherited)  value |= 0x20;
    if (flags->is_part)       value |= 0x40;

    return value;
}
