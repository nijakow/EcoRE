#include <assert.h>
#include <string.h>

#include "object.h"

#include "object_payload.h"
#include "type.h"
#include "../../vm/core/send.h"
#include "../../vm/memory/memory.h"
#include "../../vm/memory/gc/gc.h"


struct Eco_Object* Eco_OBJECTS = NULL;


struct Eco_Object*  Eco_Object_New()
{
    return Eco_Object_New_Derived(Eco_Type_PLAIN_OBJECT_TYPE, sizeof(struct Eco_Object), 0);
}

void* Eco_Object_New_Derived(struct Eco_Type* type,
                             unsigned int size,
                             unsigned int payload_size)
{
    struct Eco_Object* object;

    object = Eco_Memory_Alloc(size);

    Eco_Type_Incr(type);

    object->type                = type;

    object->header.mark_queued  = false;
    object->header.mark_done    = false;
    object->header.object_size  = size;

    object->payload             = Eco_Object_Payload_New(payload_size);

    object->next                = Eco_OBJECTS;
    Eco_OBJECTS                 = object;

    return object;
}

bool Eco_Object_Send(struct Eco_Message* message, struct Eco_Object* target)
{
    unsigned int           i;
    Eco_Any                value;
    struct Eco_Type*       type;
    struct Eco_Type_Slot*  slot;


    if (target == NULL) return false;

    type = target->type;

    for (i = 0; i < type->slot_count; i++)
    {
        if (type->slots[i].key == message->key) {
            Eco_Type_Slot_Invoke(message, target, &(type->slots[i]));
            return true;
        }
    }

    for (i = 0; i < type->slot_count; i++)
    {
        slot = &(type->slots[i]);

        switch (slot->type)
        {
            case Eco_Type_Slot_Type_INLINED:
                if (slot->body.inlined.is_inherited) {
                    if (Eco_Type_Slot_GetValue(slot, target, &value)) {
                        if (Eco_Send(message, &value)) {
                            return true;
                        }
                    }
                }
                break;
            default:
                break;
        }
    }

    return false;
}

void Eco_Object_Mark(struct Eco_GC_State* state, struct Eco_Object* object)
{
    /*
     * It's easy to confuse Eco_Object_Mark(...) and Eco_GC_State_MarkObject(...)!
     * This simple assert statement ensures that this function is only called by
     * Eco_GC_State_MarkObject(...) - as it should be!
     */
    assert(object->header.mark_queued);
}

void Eco_Object_Del(struct Eco_Object* object)
{
    if (object->payload != NULL) {
        Eco_Object_Payload_Delete(object->payload);
    }

    Eco_Type_Decr(object->type);

    Eco_Memory_Free(object);
}


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



/*
 *    T y p e C o r e
 */

struct Eco_TypeCore Eco_Object_TYPECORE;

void Eco_Object_Init()
{
    Eco_TypeCore_Create(&Eco_Object_TYPECORE, "Eco_Object");
}

void Eco_Object_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_Object_TYPECORE);
}
