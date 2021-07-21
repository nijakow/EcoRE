#ifndef ECO_OBJECTS_BASE_SLOT_INFO_H
#define ECO_OBJECTS_BASE_SLOT_INFO_H

struct Eco_Object_SlotInfo
{
    struct Eco_Object*  key;
    unsigned int        is_inherited : 1;
};

#endif
