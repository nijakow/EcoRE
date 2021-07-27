#include "object_slots.h"

#include "object.h"
#include "type.h"
#include "../misc/key/key.h"


static void Eco_Object_ResizePayload(struct Eco_Object* object, unsigned int new_size)
{
    object->payload = Eco_Object_Payload_Resize(object->payload, new_size);
}

static void Eco_Object_SwitchType(struct Eco_Object* object, struct Eco_Type* new_type)
{
    struct Eco_Type*  old_type;

    if (object->type == new_type) return;

    Eco_Type_Incr(new_type);

    old_type     = object->type;
    object->type = new_type;

    Eco_Object_ResizePayload(object, new_type->instance_payload_size);

    Eco_Type_Decr(old_type);
}

bool Eco_Object_AddSlot(struct Eco_Object* self,
                        int pos,
                        struct Eco_Object_SlotInfo info,
                        Eco_Any* value)
{
    struct Eco_Type*       new_type;
    struct Eco_Type_Slot*  slot;

    if (Eco_Type_CopyWithNewInlinedSlot(self->type, pos, info, &new_type, &slot)) {
        Eco_Object_SwitchType(self, new_type);
        Eco_Type_Slot_SetValue(slot, self, value);
        return true;
    } else {
        return false;
    }
}

bool Eco_Object_AddCodeSlot(struct Eco_Object* self,
                            int pos,
                            struct Eco_Object_SlotInfo info,
                            struct Eco_Code* code)
{
    struct Eco_Type*  new_type;

    if (Eco_Type_CopyWithNewCodeSlot(self->type, pos, info, code, &new_type)) {
        Eco_Object_SwitchType(self, new_type);
        return true;
    } else {
        return false;
    }
}

static bool Eco_Object_AddBuiltinSlot(struct Eco_Object* self,
                                      int pos,
                                      struct Eco_Object_SlotInfo info,
                                      Eco_Builtin builtin)
{
    struct Eco_Type*  new_type;

    if (Eco_Type_CopyWithNewBuiltinSlot(self->type, pos, info, builtin, &new_type)) {
        Eco_Object_SwitchType(self, new_type);
        return true;
    } else {
        return false;
    }
}

bool Eco_Object_AddBuiltin(struct Eco_Object* self, const char* key_name, Eco_Builtin builtin)
{
    struct Eco_Key*             key;
    struct Eco_Object_SlotInfo  slot_info;

    key = Eco_Key_Find(key_name);

    if (key == NULL) return false;

    slot_info.key          = (struct Eco_Object*) key;
    slot_info.is_inherited = false;

    return Eco_Object_AddBuiltinSlot(self, -1, slot_info, builtin);
}
