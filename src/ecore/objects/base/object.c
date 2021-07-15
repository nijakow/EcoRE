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



/*
 *    T y p e C o r e
 */

struct Eco_TypeCore Eco_Object_TYPECORE;

void Eco_Object_Init()
{
    Eco_TypeCore_Create(&Eco_Object_TYPECORE, "Eco_Object");
    
    Eco_Object_TYPECORE.send = Eco_Object_Send;
    Eco_Object_TYPECORE.mark = Eco_Object_Mark;
    Eco_Object_TYPECORE.del  = Eco_Object_Del;
}

void Eco_Object_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_Object_TYPECORE);
}
