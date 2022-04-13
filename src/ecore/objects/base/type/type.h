#ifndef ECO_OBJECTS_BASE_TYPE_TYPE_H
#define ECO_OBJECTS_BASE_TYPE_TYPE_H

#include <ecore/eco.h>

#include "../object/object.h"
#include "typecore.h"
#include "slot.h"

struct Eco_Code;
struct Eco_Message;
struct Eco_Molecule;
struct Eco_Interface;

struct Eco_Type
{
    struct Eco_Object     _;

    struct Eco_TypeCore*  typecore;
    struct Eco_Object*    proxy;
    struct Eco_Interface* interface;
    struct Eco_Interface* public_interface;

    struct Eco_Molecule*  implementing_molecules;

    unsigned int          slot_count;
    unsigned int          instance_payload_size;
    struct Eco_TypeSlot   slots[];
};

struct Eco_Type* Eco_Type_NewPrefab(struct Eco_TypeCore*);

void Eco_Type_Subclone(struct Eco_CloneState*, struct Eco_Type*, struct Eco_Molecule*, struct Eco_Molecule*);

static inline unsigned int Eco_Type_SlotIndexOf(struct Eco_Type* type, struct Eco_TypeSlot* slot)
{
    return slot - type->slots;
}

void Eco_Types_Init();
void Eco_Types_Terminate();

#endif
