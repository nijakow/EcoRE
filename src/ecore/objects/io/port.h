#ifndef ECO_OBJECTS_IO_PORT_H
#define ECO_OBJECTS_IO_PORT_H

#include <ecore/eco.h>
#include <ecore/objects/base/object.h>

struct Eco_Port
{
    struct Eco_Object  _;
};

struct Eco_Port* Eco_Port_New();
void Eco_Port_Mark(struct Eco_GC_State*, struct Eco_Port*);
void Eco_Port_Del(struct Eco_Port*);

void Eco_Port_Init();
void Eco_Port_Terminate();

#endif
