#ifndef ECO_OBJECTS_MISC_KEY_KEY_H
#define ECO_OBJECTS_MISC_KEY_KEY_H

#include "../../../eco.h"

#include "../../base/object.h"

#include "../../../io/econnect/reader/callback.h"


struct Eco_Key
{
    struct Eco_Object  _;

    struct Eco_Key*        next;
    struct Eco_Key*        prev;

    Eco_EConnect_Callback  econnect_callback;

    char                   name[];
};

struct Eco_Key* Eco_Key_Find(const char*);

void Eco_Key_Mark(struct Eco_GC_State*, struct Eco_Key*);
void Eco_Key_Del(struct Eco_Key*);

void Eco_Key_Init();
void Eco_Key_Terminate();

#endif
