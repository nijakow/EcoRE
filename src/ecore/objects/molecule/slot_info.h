#ifndef ECO_OBJECTS_MOLECULE_SLOT_INFO_H
#define ECO_OBJECTS_MOLECULE_SLOT_INFO_H

struct Eco_Object_SlotInfo
{
    struct Eco_Key*     key;
    unsigned int        is_inherited : 1;
    unsigned int        is_delegate  : 1;
    unsigned int        is_part      : 1;
    unsigned int        is_private   : 1;
};

#endif
