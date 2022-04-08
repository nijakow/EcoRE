#ifndef ECO_OBJECTS_MOLECULE_OBJECT_SLOTS_H
#define ECO_OBJECTS_MOLECULE_OBJECT_SLOTS_H

#include <ecore/eco.h>
#include <ecore/objects/base/type/slot_info.h>

#include "molecule.h"

struct Eco_Interface;
struct Eco_Code;

bool Eco_Molecule_AddSlot(struct Eco_Molecule*, int, struct Eco_BasicSlotInfo*, struct Eco_Interface*, Eco_Any);
bool Eco_Molecule_AddCodeSlot(struct Eco_Molecule*, int, struct Eco_BasicSlotInfo*, struct Eco_Interface*, struct Eco_Code*);
bool Eco_Molecule_RemoveSlot(struct Eco_Molecule*, unsigned int);

#endif
