#pragma once

#include "../eco.h"


struct Eco_Code;



enum Eco_Type_Slot_Type
{
    Eco_Type_Slot_Type_INLINED,
    Eco_Type_Slot_Type_CODE
};


struct Eco_Type_Slot
{
    enum Eco_Type_Slot_Type  type;
    
    struct Eco_Object*       key;

    union {
        struct {
            unsigned int offset;
            /* TODO: struct Eco_Type* type; */
            bool is_inherited;
        } inlined;
        struct Eco_Code* code;
    } body;
};


struct Eco_Type
{
    unsigned int          slot_count;
    struct Eco_Type_Slot  slots[];
};
