#include <assert.h>

#include "object.h"

#include "type.h"
#include "../vm/core/send.h"
#include "../vm/memory/memory.h"
#include "../vm/memory/gc/gc.h"

struct Eco_Object* Eco_OBJECTS = NULL;


void* Eco_Object_New(struct Eco_Type* type,
                     unsigned int size,
                     unsigned int payload_size)
{
    struct Eco_Object* object;

    if (payload_size == 0) {
        object                           = Eco_Memory_Alloc(size);
        object->payload                  = NULL;
        object->header.payload_in_object = false;
    } else if (payload_size * 2 <= size) {
        object                           = Eco_Memory_Alloc(size + payload_size);
        object->payload                  = ((char*) object) + size;
        object->header.payload_in_object = true;
    } else {
        object                           = Eco_Memory_Alloc(size);
        object->payload                  = Eco_Memory_Alloc(payload_size);
        object->header.payload_in_object = false;
    }

    object->type                = type;

    object->header.mark_queued  = false;
    object->header.mark_done    = false;
    object->header.payload_size = payload_size;

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
    if ((object->payload != NULL) && (!object->header.payload_in_object)) {
        Eco_Memory_Free(object->payload);
    }

    Eco_Memory_Free(object);
}



void Eco_Object_AddSlot(struct Eco_Object* self, struct Eco_Object* key, int pos, Eco_Any value)
{
    /* TODO, FIXME, XXX! */
}
