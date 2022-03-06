#ifndef ECO_OBJECTS_IO_DIRECTORY_H
#define ECO_OBJECTS_IO_DIRECTORY_H

#include <ecore/eco.h>
#include <ecore/objects/base/object.h>


struct Eco_Directory
{
    struct Eco_Object  _;
    unsigned int       path_length;
    char               path[];
};

struct Eco_Directory* Eco_Directory_New(char*);
void Eco_Directory_Mark(struct Eco_GC_State*, struct Eco_Directory*);
void Eco_Directory_Del(struct Eco_Directory*);

void Eco_Directory_Init();
void Eco_Directory_Terminate();

#endif
