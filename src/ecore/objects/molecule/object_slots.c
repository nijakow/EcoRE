#include "object_slots.h"

#include <ecore/objects/base/object.h>
#include <ecore/objects/base/type.h>

#include "molecule.h"

#include <ecore/objects/misc/key/key.h>


static void Eco_Molecule_ResizePayload(struct Eco_Molecule* object, unsigned int new_size)
{
    object->payload = Eco_Object_Payload_Resize(object->payload, new_size);
}

static void Eco_Molecule_SwitchType(struct Eco_Molecule* object, struct Eco_Type* new_type)
{
    if (object->_.type == new_type) return;

    object->_.type = new_type;

    Eco_Molecule_ResizePayload(object, new_type->instance_payload_size);
}

bool Eco_Molecule_AddSlot(struct Eco_Molecule* self,
                          int pos,
                          struct Eco_Object_SlotInfo info,
                          Eco_Any* value)
{
    struct Eco_Type*       new_type;
    struct Eco_Type_Slot*  slot;

    if (Eco_Type_CopyWithNewInlinedSlot(self->_.type, pos, info, &new_type, &slot)) {
        Eco_Molecule_SwitchType(self, new_type);
        Eco_Type_Slot_SetValue(slot, (struct Eco_Object*) self, value);
        return true;
    } else {
        return false;
    }
}

bool Eco_Molecule_AddCodeSlot(struct Eco_Molecule* self,
                              int pos,
                              struct Eco_Object_SlotInfo info,
                              struct Eco_Code* code)
{
    struct Eco_Type*  new_type;

    if (Eco_Type_CopyWithNewCodeSlot(self->_.type, pos, info, code, &new_type)) {
        Eco_Molecule_SwitchType(self, new_type);
        return true;
    } else {
        return false;
    }
}
