#include "type.h"

#include "object.h"
#include "../vm/core/send.h"
#include "../vm/fiber.h"
#include "../vm/core/interpreter.h"
#include "../vm/memory/gc.h"
#include "key.h"
#include "group.h"
#include "code.h"
#include "closure.h"


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
            }
            return false;
        default:
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

    Eco_Object_Mark(state, &(type->_));
}

void Eco_Type_Del(struct Eco_Type* type)
{
    Eco_Object_Del(&(type->_));
}


struct Eco_Type* Eco_Type_MakePrefabType(struct Eco_Type_Shared* shared, unsigned int slots)
{
    struct Eco_Type* type;

    type = Eco_Object_New(Eco_Type_TYPE_TYPE, sizeof(struct Eco_Type) + sizeof(struct Eco_Type_Slot) * slots);

    type->shared = shared;
    type->slot_count = slots;

    return type;
}



struct Eco_Type* Eco_Type_TYPE_TYPE = NULL;
struct Eco_Type* Eco_Type_KEY_TYPE = NULL;
struct Eco_Type* Eco_Type_GROUP_TYPE = NULL;
struct Eco_Type* Eco_Type_CODE_TYPE = NULL;
struct Eco_Type* Eco_Type_CLOSURE_TYPE = NULL;


static struct Eco_Type_Shared Eco_Type_Shared_TYPE = {
    .send = (bool(*)(struct Eco_Message*, struct Eco_Object*)) Eco_Object_Send,
    .mark = (void(*)(struct Eco_GC_State*, struct Eco_Object*)) Eco_Type_Mark,
    .del  = (void(*)(struct Eco_Object*)) Eco_Type_Del
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

void Eco_Type_InitializeTypes()
{
    Eco_Type_TYPE_TYPE         = Eco_Type_MakePrefabType(&Eco_Type_Shared_TYPE, 0);
    Eco_Type_TYPE_TYPE->_.type = Eco_Type_TYPE_TYPE;

    Eco_Type_KEY_TYPE          = Eco_Type_MakePrefabType(&Eco_Type_Shared_KEY, 0);
    Eco_Type_GROUP_TYPE        = Eco_Type_MakePrefabType(&Eco_Type_Shared_GROUP, 0);
    Eco_Type_CODE_TYPE         = Eco_Type_MakePrefabType(&Eco_Type_Shared_CODE, 0);
    Eco_Type_CLOSURE_TYPE      = Eco_Type_MakePrefabType(&Eco_Type_Shared_CLOSURE, 0);
}
