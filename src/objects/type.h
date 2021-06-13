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
            /* TODO: struct Eco_Type* type; */
            bool is_inherited;
        } inlined;
        struct Eco_Code* code;
    } body;
};

bool Eco_Type_Slot_GetValue(struct Eco_Type_Slot*, struct Eco_Object*, Eco_Any*);
bool Eco_Type_Slot_Invoke(struct Eco_Message*, struct Eco_Object*, struct Eco_Type_Slot*);


struct Eco_Type_Shared
{
    bool (*send)(struct Eco_Message*, struct Eco_Object*);
    void (*mark)(struct Eco_GC_State*, struct Eco_Object*);
    void (*del)(struct Eco_Object*);
};

struct Eco_Type
{
    struct Eco_Object        _;

    struct Eco_Type_Shared*  shared;

    unsigned int             slot_count;
    struct Eco_Type_Slot     slots[];
};

void Eco_Type_Mark(struct Eco_GC_State*, struct Eco_Type*);


extern struct Eco_Type* Eco_Type_TYPE_TYPE;
extern struct Eco_Type* Eco_Type_GROUP_TYPE;
extern struct Eco_Type* Eco_Type_CODE_TYPE;
extern struct Eco_Type* Eco_Type_CLOSURE_TYPE;

void Eco_Type_InitializeTypes();
