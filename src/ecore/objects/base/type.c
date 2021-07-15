#include "type.h"

#include "object.h"
#include "../group/group.h"
#include "../misc/key/key.h"
#include "../vm/code/code.h"
#include "../vm/code/closure.h"
#include "../../vm/core/send.h"
#include "../../vm/fiber.h"
#include "../../vm/core/interpreter.h"
#include "../../vm/memory/memory.h"
#include "../../vm/memory/gc/gc.h"


struct Eco_Type*  Eco_TYPES = NULL;



bool Eco_Type_Slot_GetValue(struct Eco_Type_Slot* slot, struct Eco_Object* object, Eco_Any* location)
{
    switch (slot->type)
    {
        case Eco_Type_Slot_Type_INLINED:
            *location = *((Eco_Any*) Eco_Object_At(object, slot->body.inlined.offset));
            return true;
        default:
            return false;
    }
}

bool Eco_Type_Slot_SetValue(struct Eco_Type_Slot* slot, struct Eco_Object* object, Eco_Any* value)
{
    switch (slot->type)
    {
        case Eco_Type_Slot_Type_INLINED:
            *((Eco_Any*) Eco_Object_At(object, slot->body.inlined.offset)) = *value;
            return true;
        default:
            return false;
    }
}

bool Eco_Type_Slot_Invoke(struct Eco_Message* message, struct Eco_Object* object, struct Eco_Type_Slot* slot)
{
    switch (message->type)
    {
        case Eco_Message_Type_SEND:
            switch (slot->type)
            {
                case Eco_Type_Slot_Type_INLINED:
                    message->body.send.arg_location[0] = *((Eco_Any*) Eco_Object_At(object, slot->body.inlined.offset));
                    return true;
                case Eco_Type_Slot_Type_CODE:
                    if (!Eco_Fiber_Enter(message->fiber, message, slot->body.code, NULL)) {
                        /* TODO: Error */
                        return false;
                    }
                    return true;
            }
            return false;
        case Eco_Message_Type_ASSIGN:
            switch (slot->type)
            {
                case Eco_Type_Slot_Type_INLINED:
                    *((Eco_Any*) Eco_Object_At(object, slot->body.inlined.offset)) = *(message->body.assign.value);
                    return true;
                default:
                    return false;
            }
        default:
            return false;
    }
}


static struct Eco_Type* Eco_Type_New(unsigned int slot_count)
{
    struct Eco_Type*  type;
    
    type                        = Eco_Memory_Alloc(sizeof(struct Eco_Type) + sizeof(struct Eco_Type_Slot) * slot_count);
    type->header.refcount       = 0;
    type->header.persistent     = false;

    type->shared                = NULL;
    type->slot_count            = slot_count;
    type->instance_payload_size = 0;

    type->header.prev           = NULL;
    type->header.next           = Eco_TYPES;
    if (Eco_TYPES != NULL) {
        Eco_TYPES->header.prev  = type;
    }
    Eco_TYPES                   = type;

    return type;
}

void Eco_Type_Del(struct Eco_Type* type)
{
    if (Eco_TYPES == type) {
        if (type->header.prev != NULL) Eco_TYPES = type->header.prev;
        else                           Eco_TYPES = type->header.next;
    }

    if (type->header.prev != NULL) type->header.prev->header.next = type->header.next;
    if (type->header.next != NULL) type->header.next->header.prev = type->header.prev;

    Eco_Memory_Free(type);
}

static struct Eco_Type* Eco_Type_New_Prefab(struct Eco_Type_Shared* shared, unsigned int slots)
{
    struct Eco_Type* type;

    type = Eco_Type_New(slots);

    type->header.persistent     = true;
    type->shared                = shared;
    type->instance_payload_size = 0;

    return type;
}

static bool Eco_Type_CopyWithNewSlot(struct Eco_Type* self,
                                     int pos,
                                     struct Eco_Type** type_loc,
                                     struct Eco_Type_Slot** slot_loc)
{
    unsigned int      i;
    unsigned int      adjusted_pos;
    struct Eco_Type*  the_copy;

    const unsigned int  new_slot_count = self->slot_count + 1;

    if (pos >= 0) adjusted_pos =  pos;
    else          adjusted_pos = -pos; 

    if (adjusted_pos >= new_slot_count) adjusted_pos = new_slot_count - 1;


    the_copy                        = Eco_Type_New(new_slot_count);
    *type_loc                       = the_copy;
    the_copy->shared                = self->shared;
    the_copy->instance_payload_size = self->instance_payload_size;

    for (i = 0; i < new_slot_count; i++) {
        if (i < adjusted_pos) the_copy->slots[i] = self->slots[i];
        else if (i == adjusted_pos) *slot_loc = &the_copy->slots[i];
        else the_copy->slots[i] = self->slots[i - 1];
    }

    *type_loc = the_copy;

    return true;
}

bool Eco_Type_CopyWithNewInlinedSlot(struct Eco_Type* self,
                                     int pos,
                                     struct Eco_Object_SlotInfo info,
                                     /* TODO: Flags */
                                     struct Eco_Type** type_loc,
                                     struct Eco_Type_Slot** slot_loc)
{
    struct Eco_Type*       the_copy;
    struct Eco_Type_Slot*  the_slot;

    const unsigned int     slot_value_size = sizeof(Eco_Any);

    if (Eco_Type_CopyWithNewSlot(self, pos, &the_copy, &the_slot)) {
        the_copy->instance_payload_size += slot_value_size;

        the_slot->type                      = Eco_Type_Slot_Type_INLINED;
        the_slot->key                       = info.key;
        the_slot->body.inlined.is_inherited = info.is_inherited;
        the_slot->body.inlined.value_size   = slot_value_size;
        the_slot->body.inlined.offset       = the_copy->instance_payload_size - slot_value_size;

        *type_loc                           = the_copy;
        *slot_loc                           = the_slot;
        
        return true;
    } else {
        return false;
    }
}

bool Eco_Type_CopyWithNewCodeSlot(struct Eco_Type* self,
                                  int pos,
                                  struct Eco_Object_SlotInfo info,
                                  struct Eco_Code* code,
                                  struct Eco_Type** type_loc)
{
    struct Eco_Type*       the_copy;
    struct Eco_Type_Slot*  the_slot;

    const unsigned int     slot_value_size = sizeof(Eco_Any);

    if (Eco_Type_CopyWithNewSlot(self, pos, &the_copy, &the_slot)) {
        the_copy->instance_payload_size += slot_value_size;

        the_slot->type      = Eco_Type_Slot_Type_CODE;
        the_slot->key       = info.key;
        the_slot->body.code = code;

        *type_loc           = the_copy;

        return true;
    } else {
        return false;
    }
}


void Eco_Type_Mark(struct Eco_GC_State* state, struct Eco_Type* type)
{
    unsigned int  i;

    for (i = 0; i < type->slot_count; i++)
    {
        switch (type->slots[i].type)
        {
            case Eco_Type_Slot_Type_INLINED:
                break;
            case Eco_Type_Slot_Type_CODE:
                Eco_Code_Mark(state, type->slots[i].body.code);
                break;
        }
    }
}

void Eco_Type_MarkTypes(struct Eco_GC_State* state)
{
    struct Eco_Type*  type;

    type = Eco_TYPES;
    while (type != NULL)
    {
        Eco_Type_Mark(state, type);
        type = type->header.next;
    }
}


struct Eco_Type* Eco_Type_TYPE_TYPE = NULL;
struct Eco_Type* Eco_Type_PLAIN_OBJECT_TYPE = NULL;
struct Eco_Type* Eco_Type_KEY_TYPE = NULL;
struct Eco_Type* Eco_Type_GROUP_TYPE = NULL;
struct Eco_Type* Eco_Type_CODE_TYPE = NULL;
struct Eco_Type* Eco_Type_CLOSURE_TYPE = NULL;


static struct Eco_Type_Shared Eco_Type_Shared_PLAIN_OBJECT = {
    .send = (bool(*)(struct Eco_Message*, struct Eco_Object*)) Eco_Object_Send,
    .mark = (void(*)(struct Eco_GC_State*, struct Eco_Object*)) Eco_Object_Mark,
    .del  = (void(*)(struct Eco_Object*)) Eco_Object_Del
};
static struct Eco_Type_Shared Eco_Type_Shared_KEY = {
    .send = (bool(*)(struct Eco_Message*, struct Eco_Object*)) Eco_Object_Send,
    .mark = (void(*)(struct Eco_GC_State*, struct Eco_Object*)) Eco_Key_Mark,
    .del  = (void(*)(struct Eco_Object*)) Eco_Key_Del
};
static struct Eco_Type_Shared Eco_Type_Shared_GROUP = {
    .send = (bool(*)(struct Eco_Message*, struct Eco_Object*)) Eco_Group_Send,
    .mark = (void(*)(struct Eco_GC_State*, struct Eco_Object*)) Eco_Group_Mark,
    .del  = (void(*)(struct Eco_Object*)) Eco_Group_Del
};
static struct Eco_Type_Shared Eco_Type_Shared_CODE = {
    .send = (bool(*)(struct Eco_Message*, struct Eco_Object*)) Eco_Object_Send,
    .mark = (void(*)(struct Eco_GC_State*, struct Eco_Object*)) Eco_Code_Mark,
    .del  = (void(*)(struct Eco_Object*)) Eco_Code_Del
};
static struct Eco_Type_Shared Eco_Type_Shared_CLOSURE = {
    .send = (bool(*)(struct Eco_Message*, struct Eco_Object*)) Eco_Object_Send,
    .mark = (void(*)(struct Eco_GC_State*, struct Eco_Object*)) Eco_Closure_Mark,
    .del  = (void(*)(struct Eco_Object*)) Eco_Closure_Del
};

void Eco_Type_CreateTypes()
{
    Eco_Type_PLAIN_OBJECT_TYPE = Eco_Type_New_Prefab(&Eco_Type_Shared_PLAIN_OBJECT, 0);
    Eco_Type_KEY_TYPE          = Eco_Type_New_Prefab(&Eco_Type_Shared_KEY, 0);
    Eco_Type_GROUP_TYPE        = Eco_Type_New_Prefab(&Eco_Type_Shared_GROUP, 0);
    Eco_Type_CODE_TYPE         = Eco_Type_New_Prefab(&Eco_Type_Shared_CODE, 0);
    Eco_Type_CLOSURE_TYPE      = Eco_Type_New_Prefab(&Eco_Type_Shared_CLOSURE, 0);
}

void Eco_Type_DestroyTypes()
{
    Eco_Type_Del(Eco_Type_PLAIN_OBJECT_TYPE);
    Eco_Type_Del(Eco_Type_KEY_TYPE);
    Eco_Type_Del(Eco_Type_GROUP_TYPE);
    Eco_Type_Del(Eco_Type_CODE_TYPE);
    Eco_Type_Del(Eco_Type_CLOSURE_TYPE);
}
