#ifndef ECO_OBJECTS_BASE_TYPE_TYPE_H
#define ECO_OBJECTS_BASE_TYPE_TYPE_H

#include <ecore/eco.h>

#include "../object/object.h"
#include "typecore.h"
#include "slot.h"

struct Eco_Code;
struct Eco_Message;
struct Eco_GC_State;
struct Eco_Molecule;
struct Eco_Interface;

struct Eco_Type
{
    struct Eco_Object     _;

    struct Eco_TypeCore*  typecore;
    struct Eco_Object*    proxy;
    struct Eco_Interface* interface;
    struct Eco_Interface* public_interface;

    unsigned int          slot_count;
    unsigned int          instance_payload_size;
    struct Eco_TypeSlot   slots[];
};

struct Eco_Type* Eco_Type_NewPrefab(struct Eco_TypeCore*);

void Eco_Type_MarkMolecule(struct Eco_GC_State*, struct Eco_Type*, struct Eco_Molecule*);
void Eco_Type_Subclone(struct Eco_CloneState*, struct Eco_Type*, struct Eco_Molecule*, struct Eco_Molecule*);

void Eco_Types_Init();
void Eco_Types_Terminate();

#endif
