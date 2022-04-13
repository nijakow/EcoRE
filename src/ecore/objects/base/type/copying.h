#ifndef ECO_OBJECTS_BASE_TYPE_COPYING_H
#define ECO_OBJECTS_BASE_TYPE_COPYING_H

#include "type.h"

bool Eco_Type_CopyWithNewInlinedSlot(struct Eco_Type*, int, struct Eco_BasicSlotInfo*, struct Eco_Interface*, struct Eco_Type**, struct Eco_TypeSlot**);
bool Eco_Type_CopyWithNewCodeSlot(struct Eco_Type*, int, struct Eco_BasicSlotInfo*, struct Eco_Interface*, struct Eco_Code*, struct Eco_Type**);
bool Eco_Type_CopyWithRemovedSlot(struct Eco_Type*, unsigned int, struct Eco_Type**);
bool Eco_Type_CopyWithChangedSlotTypeReference(struct Eco_Type*, unsigned int, struct Eco_Type*, struct Eco_Type**);

#endif
