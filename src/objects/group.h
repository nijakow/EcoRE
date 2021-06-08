#pragma once

#include "object.h"

struct Eco_Message;


struct Eco_Group
{
    struct Eco_Object    _;

    unsigned int         object_alloc;
    union {
        struct {
            unsigned int         object_count;
            struct Eco_Object**  objects;
        } multi;
        struct Eco_Object* single[2];
    } body;
};


struct Eco_Group* Eco_Group_New(struct Eco_Object*);

bool Eco_Send_ToGroup(struct Eco_Message*, struct Eco_Group*);
