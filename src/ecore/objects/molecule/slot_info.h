#ifndef ECO_OBJECTS_MOLECULE_SLOT_INFO_H
#define ECO_OBJECTS_MOLECULE_SLOT_INFO_H

struct Eco_Object_SlotInfo
{
    struct Eco_Object*  key;
    unsigned int        is_inherited : 1;
    unsigned int        is_part      : 1;
};

#endif
