#ifndef ECO_OBJECTS_BASE_TYPE_H
#define ECO_OBJECTS_BASE_TYPE_H

#include <ecore/eco.h>

#include "object.h"
#include "slot_info.h"
#include "typecore.h"


struct Eco_Code;
struct Eco_Message;
struct Eco_GC_State;
struct Eco_Molecule;



enum Eco_Type_Slot_Type
{
    Eco_Type_Slot_Type_INLINED,
    Eco_Type_Slot_Type_SHARED,
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
            bool is_part;
        } inlined;
        struct {
            bool is_inherited;
            Eco_Any value;
        } shared;
        struct Eco_Code*     code;
    } body;
};

bool Eco_Type_Slot_GetValue(struct Eco_Type_Slot*, struct Eco_Object*, Eco_Any*);
bool Eco_Type_Slot_SetValue(struct Eco_Type_Slot*, struct Eco_Object*, Eco_Any*);
bool Eco_Type_Slot_Invoke(struct Eco_Message*, struct Eco_Object*, struct Eco_Type_Slot*);



struct Eco_Type
{
    struct Eco_Object      _;

    struct Eco_TypeCore*   typecore;

    struct Eco_Object*     proxy;

    unsigned int           slot_count;
    unsigned int           instance_payload_size;
    struct Eco_Type_Slot   slots[];
};

struct Eco_Type* Eco_Type_NewPrefab(struct Eco_TypeCore*);

bool Eco_Type_CopyWithNewInlinedSlot(struct Eco_Type*, int, struct Eco_Object_SlotInfo, struct Eco_Type**, struct Eco_Type_Slot**);
bool Eco_Type_CopyWithNewCodeSlot(struct Eco_Type*, int, struct Eco_Object_SlotInfo, struct Eco_Code*, struct Eco_Type**);

void Eco_Type_MarkMolecule(struct Eco_GC_State*, struct Eco_Type*, struct Eco_Molecule*);
void Eco_Type_Subclone(struct Eco_CloneState*, struct Eco_Type*, struct Eco_Molecule*, struct Eco_Molecule*);

void Eco_Types_Init();
void Eco_Types_Terminate();

#endif
