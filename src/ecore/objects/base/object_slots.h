#pragma once

#include "../../eco.h"
#include "slot_info.h"

struct Eco_Code;


bool Eco_Object_AddSlot(struct Eco_Object*, int, struct Eco_Object_SlotInfo, Eco_Any*);
bool Eco_Object_AddCodeSlot(struct Eco_Object*, int, struct Eco_Object_SlotInfo, struct Eco_Code*);
