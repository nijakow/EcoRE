#ifndef ECO_OBJECTS_VM_INTERFACE_INTERFACE_H
#define ECO_OBJECTS_VM_INTERFACE_INTERFACE_H

#include <ecore/objects/base/object.h>
#include <ecore/objects/misc/key/key.h>

/*
 * TODO, FIXME, XXX: Make this dynamic
 */
#define Eco_InterfaceEntry_MAXARGS 8

struct Eco_Interface;

struct Eco_InterfaceEntry
{
    struct Eco_Key*        key;
    unsigned int           arg_count;
    bool                   has_varargs;
    struct Eco_Interface*  return_type;
    struct Eco_Interface*  arg_types[Eco_InterfaceEntry_MAXARGS];
};

struct Eco_Interface
{
    struct Eco_Object          _;
    struct Eco_Interface**     prev;
    struct Eco_Interface*      next;
    bool                       allow_all;
    unsigned int               entry_count;
    struct Eco_InterfaceEntry  entries[];
};

struct Eco_Interface* Eco_Interface_GetDefaultInterface();

struct Eco_Interface* Eco_Interface_New(unsigned int);
void Eco_Interface_Mark(struct Eco_GC_State*, struct Eco_Interface*);
void Eco_Interface_Del(struct Eco_Interface*);

bool Eco_Interface_ImplementsMessage(struct Eco_Interface*, struct Eco_Key*);

struct Eco_Interface* Eco_Interface_NewAndInit(unsigned int, struct Eco_InterfaceEntry*);
void Eco_Interface_AddParent(struct Eco_Interface*, struct Eco_Interface*);
struct Eco_Interface* Eco_Interface_AddEntry(struct Eco_Interface*, struct Eco_InterfaceEntry*);

void Eco_Interface_Init();
void Eco_Interface_Terminate();

#endif
