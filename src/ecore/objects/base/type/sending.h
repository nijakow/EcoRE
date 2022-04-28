#ifndef ECO_OBJECTS_BASE_TYPE_SENDING_H
#define ECO_OBJECTS_BASE_TYPE_SENDING_H

#include "type.h"

#include <ecore/vm/core/send.h>

bool Eco_Type_SendMessageToMolecule(struct Eco_Message*, Eco_Any, struct Eco_Type*, struct Eco_Molecule*, bool);

#endif
