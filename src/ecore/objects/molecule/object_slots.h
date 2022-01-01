#ifndef ECO_OBJECTS_MOLECULE_OBJECT_SLOTS_H
#define ECO_OBJECTS_MOLECULE_OBJECT_SLOTS_H

#include <ecore/eco.h>

#include "slot_info.h"
#include "molecule.h"

struct Eco_Code;

bool Eco_Molecule_AddSlot(struct Eco_Molecule*, int, struct Eco_Object_SlotInfo, Eco_Any*);
bool Eco_Molecule_AddCodeSlot(struct Eco_Molecule*, int, struct Eco_Object_SlotInfo, struct Eco_Code*);
bool Eco_Molecule_RemoveSlot(struct Eco_Molecule*, unsigned int);

#endif
