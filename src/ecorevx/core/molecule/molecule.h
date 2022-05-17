#ifndef ECO_CORE_MOLECULE_MOLECULE_H
#define ECO_CORE_MOLECULE_MOLECULE_H

#include "../memory/memory.h"
#include "../object/object.h"
#include "../map/slotdef.h"

#define Eco_MOLECULE_SLOT_COUNT (Eco_DEFAULT_OBJECT_SLOT_COUNT)

static inline Eco_Any Eco_Molecule_Get(struct Eco_Object* self, unsigned int index)
{
    return Eco_Object_Get(self, Eco_MOLECULE_SLOT_COUNT + index);
}

static inline void Eco_Molecule_Set(struct Eco_Object* self, unsigned int index, Eco_Any value)
{
    Eco_Object_Set(self, Eco_MOLECULE_SLOT_COUNT + index, value);
}

/*
 * These functions should only be called during a safe phase!
 */

bool Eco_Molecule_AddValueSlot(Eco_Allocator_t,
                               struct Eco_Object**,
                               int,
                               struct Eco_SlotDef,
                               Eco_Any);

bool Eco_Molecule_AddCodeSlot(struct Eco_Object*,
                              int,
                              struct Eco_SlotDef,
                              struct Eco_Object*);

bool Eco_Molecule_RemoveSlot(struct Eco_Object*, int);

#endif
