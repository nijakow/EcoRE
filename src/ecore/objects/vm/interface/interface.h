#ifndef ECO_OBJECTS_VM_INTERFACE_INTERFACE_H
#define ECO_OBJECTS_VM_INTERFACE_INTERFACE_H

#include <ecore/objects/base/object.h>
#include <ecore/objects/misc/key/key.h>

struct Eco_Interface;

struct Eco_InterfaceEntry
{
    struct Eco_Key*        key;
    struct Eco_Interface*  return_type;
    unsigned int           arg_count;
    struct Eco_Interface*  arg_types[];
};

struct Eco_Interface
{
    struct Eco_Object          _;
    unsigned int               entry_count;
    struct Eco_InterfaceEntry  entries[];
};

struct Eco_Interface* Eco_Interface_New(unsigned int);
void Eco_Interface_Mark(struct Eco_GC_State*, struct Eco_Interface*);
void Eco_Interface_Del(struct Eco_Interface*);

struct Eco_Interface* Eco_Interface_AddEntry(struct Eco_Interface*, struct Eco_InterfaceEntry*);

void Eco_Interface_Init();
void Eco_Interface_Terminate();

#endif
