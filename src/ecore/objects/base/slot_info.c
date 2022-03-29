#include <ecore/base/defs.h>

#include "slot_info.h"


void Eco_SlotFlags_Create(struct Eco_SlotFlags* flags)
{
    flags->is_deprecated = false;
    flags->is_private    = false;
    flags->is_final      = false;
    flags->is_inherited  = false;
    flags->is_delegate   = false;
    flags->is_part       = false;
}

void Eco_BasicSlotInfo_Create(struct Eco_BasicSlotInfo* info)
{
    info->key = NULL;
    Eco_SlotFlags_Create(&info->flags);
}
