#ifndef ECO_OBJECTS_BASE_TYPE_H
#define ECO_OBJECTS_BASE_TYPE_H

#include "../../eco.h"

#include "builtin.h"
#include "object.h"
#include "slot_info.h"
#include "typecore.h"


struct Eco_Code;
struct Eco_Message;
struct Eco_GC_State;



enum Eco_Type_Slot_Type
{
    Eco_Type_Slot_Type_INLINED,
    Eco_Type_Slot_Type_CODE,
    Eco_Type_Slot_Type_BUILTIN
};


struct Eco_Type_Slot
{
    enum Eco_Type_Slot_Type  type;
    
    struct Eco_Object*       key;

    union {
        struct {
            unsigned int offset;
            unsigned int value_size;
            bool is_inherited;
        } inlined;
        struct Eco_Code*     code;
        Eco_Builtin          builtin;
    } body;
};

bool Eco_Type_Slot_GetValue(struct Eco_Type_Slot*, struct Eco_Object*, Eco_Any*);
bool Eco_Type_Slot_SetValue(struct Eco_Type_Slot*, struct Eco_Object*, Eco_Any*);
bool Eco_Type_Slot_Invoke(struct Eco_Message*, struct Eco_Object*, struct Eco_Type_Slot*);



struct Eco_Type
{
    struct {
        struct Eco_Type*  next;
        struct Eco_Type*  prev;
        unsigned int      refcount;
        bool              persistent;
    }                     header;

    struct Eco_TypeCore*  typecore;

    unsigned int          slot_count;
    unsigned int          instance_payload_size;
    struct Eco_Type_Slot  slots[];
};

void Eco_Type_Del(struct Eco_Type*);

static inline void Eco_Type_Incr(struct Eco_Type* type)
{
    type->header.refcount++;
}

static inline void Eco_Type_Decr(struct Eco_Type* type)
{
    if (type->header.refcount <= 1) {
        if (!type->header.persistent) {
            Eco_Type_Del(type);
        }
    } else {
        type->header.refcount--;
    }
}


struct Eco_Type* Eco_Type_NewPrefab(struct Eco_TypeCore*);

bool Eco_Type_CopyWithNewInlinedSlot(struct Eco_Type*, int, struct Eco_Object_SlotInfo, struct Eco_Type**, struct Eco_Type_Slot**);
bool Eco_Type_CopyWithNewCodeSlot(struct Eco_Type*, int, struct Eco_Object_SlotInfo, struct Eco_Code*, struct Eco_Type**);
bool Eco_Type_CopyWithNewBuiltinSlot(struct Eco_Type*, int, struct Eco_Object_SlotInfo, Eco_Builtin, struct Eco_Type**);

void Eco_Type_Mark(struct Eco_GC_State*, struct Eco_Type*);
void Eco_Type_MarkTypes(struct Eco_GC_State*);

void Eco_Types_Init();
void Eco_Types_Terminate();

#endif
