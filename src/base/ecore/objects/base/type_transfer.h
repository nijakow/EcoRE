#ifndef ECO_OBJECTS_BASE_TYPE_TRANSFER_H
#define ECO_OBJECTS_BASE_TYPE_TRANSFER_H

#include <ecore/objects/base/type/type.h>
#include <ecore/objects/base/molecule/molecule.h>

void Eco_TypeTransfer_Patch();
void Eco_TypeTransfer(struct Eco_Molecule*, unsigned int, struct Eco_Type*);

#endif
