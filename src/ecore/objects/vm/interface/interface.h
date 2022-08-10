#ifndef ECO_OBJECTS_VM_INTERFACE_INTERFACE_H
#define ECO_OBJECTS_VM_INTERFACE_INTERFACE_H

#include <ecore/objects/base/object.h>
#include <ecore/objects/base/type/typelist.h>
#include <ecore/objects/misc/key/key.h>

/*
 * TODO, FIXME, XXX: Make this dynamic
 */
#define Eco_InterfaceEntry_MAXARGS 8

struct Eco_Interface;
struct Eco_Array;

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
    struct Eco_Object           _;
    struct Eco_Interface**      prev;
    struct Eco_Interface*       next;
    struct Eco_TypeList         implementing_types;
    bool                        allow_all;
    unsigned int                parent_count;
    struct Eco_Interface**      parents;
    unsigned int                entry_count;
    struct Eco_InterfaceEntry*  entries;
    char                        payload[0];
};

static inline unsigned int Eco_Interface_GetParentCount(struct Eco_Interface* interface)
{
    return interface->parent_count;
}

static inline unsigned int Eco_Interface_GetEntryCount(struct Eco_Interface* interface)
{
    return interface->entry_count;
}

struct Eco_Interface* Eco_Interface_GetDefaultInterface();

struct Eco_Interface* Eco_Interface_New(unsigned int, unsigned int);
void Eco_Interface_Mark(struct Eco_GC_State*, struct Eco_Interface*);
void Eco_Interface_Del(struct Eco_Interface*);

bool Eco_Interface_ImplementsMessage(struct Eco_Interface*, struct Eco_Key*);
bool Eco_Interface_ImplementsInterface(struct Eco_Interface*, struct Eco_Interface*);

void Eco_Interface_NotifyImplementedBy(struct Eco_Interface*, struct Eco_Type*);
void Eco_Interface_NotifyNotImplementedBy(struct Eco_Interface*, struct Eco_Type*);

struct Eco_Interface* Eco_Interface_NewAndInit(unsigned int, struct Eco_InterfaceEntry*);
struct Eco_Interface* Eco_Interface_AddParent(struct Eco_Interface*, struct Eco_Interface*);
struct Eco_Interface* Eco_Interface_AddEntry(struct Eco_Interface*, struct Eco_InterfaceEntry*);

struct Eco_Array* Eco_Interface_GetAllImplementingTypesAsArray(struct Eco_Interface*);

struct Eco_Array* Eco_Interface_GetAllInterfacesAsArray();

void Eco_Interface_Init();
void Eco_Interface_Terminate();

#endif
