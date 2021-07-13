#pragma once

#include "../eco.h"
#include "object.h"


struct Eco_Code;
struct Eco_Message;
struct Eco_GC_State;



enum Eco_Type_Slot_Type
{
    Eco_Type_Slot_Type_INLINED,
    Eco_Type_Slot_Type_CODE
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
        struct Eco_Code* code;
    } body;
};

bool Eco_Type_Slot_GetValue(struct Eco_Type_Slot*, struct Eco_Object*, Eco_Any*);
bool Eco_Type_Slot_SetValue(struct Eco_Type_Slot*, struct Eco_Object*, Eco_Any*);
bool Eco_Type_Slot_Invoke(struct Eco_Message*, struct Eco_Object*, struct Eco_Type_Slot*);


struct Eco_Type_Shared
{
    bool (*send)(struct Eco_Message*, struct Eco_Object*);
    void (*mark)(struct Eco_GC_State*, struct Eco_Object*);
    void (*del)(struct Eco_Object*);
};

struct Eco_Type
{
    struct {
        struct Eco_Type* next;
        struct Eco_Type* prev;
        unsigned int     refcount;
        bool             persistent;
    } header;

    struct Eco_Type_Shared*  shared;

    unsigned int             slot_count;
    unsigned int             instance_payload_size;
    struct Eco_Type_Slot     slots[];
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

bool Eco_Type_CopyWithNewInlinedSlot(struct Eco_Type*, int, struct Eco_Object* key, struct Eco_Type**, struct Eco_Type_Slot**);

void Eco_Type_Mark(struct Eco_GC_State*, struct Eco_Type*);


extern struct Eco_Type* Eco_Type_TYPE_TYPE;
extern struct Eco_Type* Eco_Type_PLAIN_OBJECT_TYPE;
extern struct Eco_Type* Eco_Type_KEY_TYPE;
extern struct Eco_Type* Eco_Type_GROUP_TYPE;
extern struct Eco_Type* Eco_Type_CODE_TYPE;
extern struct Eco_Type* Eco_Type_CLOSURE_TYPE;

void Eco_Type_MarkTypes(struct Eco_GC_State*);
void Eco_Type_CreateTypes();
void Eco_Type_DestroyTypes();
