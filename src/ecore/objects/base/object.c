#include <assert.h>
#include <string.h>

#include "object.h"

#include "object_payload.h"
#include "type.h"

#include <ecore/vm/core/clone.h>
#include <ecore/vm/core/send.h>
#include <ecore/vm/memory/memory.h>
#include <ecore/vm/memory/gc/gc.h>


/*
 *    B o o k k e e p i n g   a n d   T y p e
 */

struct Eco_Arena Eco_OBJECTS;

static struct Eco_TypeCore Eco_Object_TYPECORE;
static struct Eco_Type*    Eco_Object_TYPE;

void Eco_Object_Init()
{
    Eco_Arena_Create(&Eco_OBJECTS);

    Eco_TypeCore_Create(&Eco_Object_TYPECORE, "Eco_Object");
    
    Eco_Object_TYPECORE.send  = Eco_Object_Send;
    Eco_Object_TYPECORE.mark  = Eco_Object_Mark;
    Eco_Object_TYPECORE.clone = Eco_Object_Clone;
    Eco_Object_TYPECORE.del   = Eco_Object_Del;

    Eco_Object_TYPE           = Eco_Type_NewPrefab(&Eco_Object_TYPECORE);
}

void Eco_Object_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_Object_TYPECORE);
    Eco_Arena_Destroy(&Eco_OBJECTS);
}


/*
 *    B a s i c s
 */

void* Eco_Object_NewInArena(struct Eco_Type* type,
                            unsigned int size,
                            unsigned int payload_size,
                            struct Eco_Arena* arena)
{
    struct Eco_Object* object;

    object = Eco_Memory_Alloc(size);

    object->type                = type;

    object->header.mark_queued  = false;
    object->header.mark_done    = false;
    object->header.sticky       = false;
    object->header.object_size  = size;

    object->payload             = Eco_Object_Payload_New(payload_size);

    object->next                = arena->objects;
    arena->objects              = object;
    arena->object_count++;

    return object;
}

void* Eco_Object_New(struct Eco_Type* type,
                     unsigned int size,
                     unsigned int payload_size)
{
    return Eco_Object_NewInArena(type, size, payload_size, &Eco_OBJECTS);
}

struct Eco_Object* Eco_Object_NewPlain()
{
    return Eco_Object_New(Eco_Object_TYPE, sizeof(struct Eco_Object), 0);
}


bool Eco_Object_Send(struct Eco_Message* message,
                     struct Eco_SendLink* link,
                     struct Eco_Object* target)
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
                        if (Eco_Send(message, link, &value)) {
                            return true;
                        }
                    }
                }
                break;
            case Eco_Type_Slot_Type_SHARED:
                if (slot->body.shared.is_inherited) {
                    return Eco_Send(message, link, &slot->body.shared.value);
                }
                break;
            default:
                break;
        }
    }

    if (target->type->proxy != NULL) {
        return Eco_Send_ToObject(message, link, target->type->proxy);
    } else {
        return false;
    }
}

void Eco_Object_Mark(struct Eco_GC_State* state, struct Eco_Object* object)
{
    /*
     * It's easy to confuse Eco_Object_Mark(...) and Eco_GC_State_MarkObject(...)!
     * Only call Eco_Object_Mark(...) as the last function in a Eco_<Type>_Mark(...)!
     */
    Eco_Type_MarkObject(state, object->type, object);
    Eco_GC_State_MarkObject(state, object->type);
}

struct Eco_Object* Eco_Object_Clone(struct Eco_CloneState* state, struct Eco_Object* object)
{
    struct Eco_Object*  clone;

    clone = Eco_CloneState_QueryClone(state, object);

    if (clone == NULL) {
        clone = Eco_Object_New(object->type, object->header.object_size, object->payload->size);
        Eco_CloneState_RegisterClone(state, object, clone);
        Eco_Type_Subclone(state, object->type, object, clone);
    }

    return clone;
}

struct Eco_Object* Eco_Object_NoClone(struct Eco_CloneState* state,
                                      struct Eco_Object* original)
{
    return original;
}

void Eco_Object_Del(struct Eco_Object* object)
{
    if (object->payload != NULL) {
        Eco_Object_Payload_Delete(object->payload);
    }

    Eco_Memory_Free(object);
}
