#ifndef ECO_OBJECTS_BASE_TYPE_GC_H
#define ECO_OBJECTS_BASE_TYPE_GC_H

#include "type.h"

struct Eco_GC_State;
struct Eco_Molecule;

void Eco_Type_Mark(struct Eco_GC_State*, struct Eco_Type*);
void Eco_Type_MarkMolecule(struct Eco_GC_State*, struct Eco_Type*, struct Eco_Molecule*);

#endif
