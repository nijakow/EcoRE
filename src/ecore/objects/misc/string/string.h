#ifndef ECO_OBJECTS_MISC_STRING_STRING_H
#define ECO_OBJECTS_MISC_STRING_STRING_H

#include <ecore/eco.h>

#include <ecore/objects/base/object.h>


struct Eco_String
{
    struct Eco_Object  _;
    char               bytes[];
};

struct Eco_String* Eco_String_New(const char*);
void Eco_String_Mark(struct Eco_GC_State*, struct Eco_String*);
void Eco_String_Del(struct Eco_String*);

void Eco_String_Init();
void Eco_String_Terminate();

#endif
