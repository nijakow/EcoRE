#include "object_slots.h"
#include "molecule.h"

#include <ecore/objects/base/object.h>
#include <ecore/objects/base/type/type.h>
#include <ecore/objects/base/type/copying.h>

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

bool Eco_Molecule_AddSlot(struct Eco_Molecule*      self,
                          int                       pos,
                          struct Eco_BasicSlotInfo* info,
                          struct Eco_Interface*     interface,
                          Eco_Any*                  value)  // TODO: Change this to Eco_Any (no pointer)
{
    struct Eco_Type*      new_type;
    struct Eco_TypeSlot*  slot;

    if (Eco_Type_CopyWithNewInlinedSlot(self->_.type, pos, info, interface, &new_type, &slot)) {
        Eco_Molecule_SwitchType(self, new_type);
        Eco_TypeSlot_SetValue(slot, self, *value);
        return true;
    } else {
        return false;
    }
}

bool Eco_Molecule_AddCodeSlot(struct Eco_Molecule*      self,
                              int                       pos,
                              struct Eco_BasicSlotInfo* info,
                              struct Eco_Interface*     interface,
                              struct Eco_Code*          code)
{
    struct Eco_Type*  new_type;

    if (Eco_Type_CopyWithNewCodeSlot(self->_.type, pos, info, interface, code, &new_type)) {
        Eco_Molecule_SwitchType(self, new_type);
        return true;
    } else {
        return false;
    }
}

bool Eco_Molecule_RemoveSlot(struct Eco_Molecule* self, unsigned int pos)
{
    struct Eco_Type*  new_type;

    if (Eco_Type_CopyWithRemovedSlot(self->_.type, pos, &new_type)) {
        Eco_Molecule_SwitchType(self, new_type);
        return true;
    } else {
        return false;
    }
}
