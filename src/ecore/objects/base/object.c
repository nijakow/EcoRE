#include <assert.h>
#include <string.h>

#include "object.h"

#include "type.h"

#include <ecore/objects/vm/interface/interface.h>
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
    
    Eco_Object_TYPECORE.send  = NULL;
    Eco_Object_TYPECORE.mark  = Eco_Object_Mark;
    Eco_Object_TYPECORE.clone = Eco_Object_NoClone;
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
                            struct Eco_Arena* arena)
{
    struct Eco_Object* object;

    object = Eco_Memory_Alloc(size);

    object->type              = type;
    object->up                = NULL;

    object->bits.mark_queued  = false;
    object->bits.mark_done    = false;
    object->bits.sticky       = false;

    object->next              = arena->objects;
    arena->objects            = object;
    arena->object_count++;

    return object;
}

void* Eco_Object_New(struct Eco_Type* type,
                     unsigned int size)
{
    return Eco_Object_NewInArena(type, size, &Eco_OBJECTS);
}

struct Eco_Object* Eco_Object_NewPlain()
{
    return Eco_Object_New(Eco_Object_TYPE, sizeof(struct Eco_Object));
}



void Eco_Object_Mark(struct Eco_GC_State* state, struct Eco_Object* object)
{
    /*
     * It's easy to confuse Eco_Object_Mark(...) and Eco_GC_State_MarkObject(...)!
     * Only call Eco_Object_Mark(...) as the last function in a Eco_<Type>_Mark(...)!
     */
    Eco_GC_State_MarkObject(state, object->type);
    if (object->up != NULL)
        Eco_GC_State_MarkObject(state, object->up);
}

bool Eco_Object_Send(struct Eco_Message* message,
                     struct Eco_SendLink* link,
                     struct Eco_Object* target,
                     Eco_Any* self)
{
    unsigned int          i;
    Eco_Any               value;
    struct Eco_Type*      type;
    struct Eco_TypeSlot*  slot;
    bool                  result;


    if (target == NULL) return false;

    type = target->type;

    for (i = 0; i < type->slot_count; i++)
    {
        slot = &type->slots[i];
        if (slot->key == message->key) {
            Eco_TypeSlot_Invoke(message, target, slot, self);
            return true;
        }
    }

    for (i = 0; i < type->slot_count; i++)
    {
        slot = &(type->slots[i]);

        switch (slot->type)
        {
            case Eco_TypeSlotType_INLINED:
                if (slot->flags.is_inherited && Eco_Interface_ImplementsMessage(slot->interface, message->key)) {
                    if (Eco_TypeSlot_GetValue(slot, target, &value)) {
                        if (slot->flags.is_delegate) {
                            result = Eco_Send(message, link, &value, &value);
                        } else {
                            result = Eco_Send(message, link, &value, self);
                        }
                        if (result) return true;
                    }
                }
                break;
            case Eco_TypeSlotType_SHARED:
                if (slot->flags.is_inherited && Eco_Interface_ImplementsMessage(slot->interface, message->key)) {
                    //if (slot->body.shared.is_delegate) {  // TODO
                        result = Eco_Send(message, link, &slot->body.shared.value, &slot->body.shared.value);
                    /*} else {
                        result = Eco_Send(message, link, &slot->body.shared.value, self);
                    }*/
                    if (result) return true;
                }
                break;
            default:
                break;
        }
    }

    if (target->type->proxy != NULL) {
        return Eco_Send_ToObject(message, link, target->type->proxy, self);
    } else {
        return false;
    }
}

struct Eco_Object* Eco_Object_NoClone(struct Eco_CloneState* state,
                                      struct Eco_Object* original,
                                      bool forced)
{
    return original;
}

void Eco_Object_Del(struct Eco_Object* object)
{
    Eco_Memory_Free(object);
}


bool Eco_Object_IsBlessedBy(struct Eco_Object* object, struct Eco_Object* root)
{
    if (root != NULL) {
        while (object != NULL)
        {
            if (object->up == root)
                return true;
            object = object->up;
        }
    }
    return false;
}
