#ifndef ECO_OBJECTS_BASE_TYPE_H
#define ECO_OBJECTS_BASE_TYPE_H

#include <ecore/eco.h>

#include "object.h"
#include "typecore.h"

#include <ecore/objects/molecule/slot_info.h>


struct Eco_Code;
struct Eco_Message;
struct Eco_GC_State;
struct Eco_Molecule;



enum Eco_TypeSlotType
{
    Eco_TypeSlotType_INLINED,
    Eco_TypeSlotType_SHARED,
    Eco_TypeSlotType_CODE
};


struct Eco_TypeSlotFlags
{
    int is_deprecated : 1;
    int is_private    : 1;
    int is_final      : 1;
    int is_inherited  : 1;
    int is_delegate   : 1;
    int is_part       : 1;
};

void Eco_TypeSlot_Flags_Initialize(struct Eco_TypeSlotFlags*);


struct Eco_TypeSlot
{
    enum   Eco_TypeSlotType   type;
    struct Eco_TypeSlotFlags  flags;
    struct Eco_Interface*     interface;
    struct Eco_Key*           key;

    union {
        struct {
            unsigned int offset;
            unsigned int value_size;
        } inlined;
        struct {
            Eco_Any value;
        } shared;
        struct {
            struct Eco_Code* code;
        } code;
    }                           body;
};

void Eco_TypeSlot_Initialize(struct Eco_TypeSlot*);

bool Eco_TypeSlot_GetValue(struct Eco_TypeSlot*, struct Eco_Object*, Eco_Any*);
bool Eco_TypeSlot_SetValue(struct Eco_TypeSlot*, struct Eco_Object*, Eco_Any*);
bool Eco_TypeSlot_Invoke(struct Eco_Message*, struct Eco_Object*, struct Eco_TypeSlot*, Eco_Any*);


struct Eco_Type
{
    struct Eco_Object     _;

    struct Eco_TypeCore*  typecore;

    struct Eco_Object*    proxy;

    unsigned int         slot_count;
    unsigned int         instance_payload_size;
    struct Eco_TypeSlot  slots[];
};

struct Eco_Type* Eco_Type_NewPrefab(struct Eco_TypeCore*);

bool Eco_Type_CopyWithNewInlinedSlot(struct Eco_Type*, int, struct Eco_Object_SlotInfo, struct Eco_Type**, struct Eco_TypeSlot**);
bool Eco_Type_CopyWithNewCodeSlot(struct Eco_Type*, int, struct Eco_Object_SlotInfo, struct Eco_Code*, struct Eco_Type**);
bool Eco_Type_CopyWithRemovedSlot(struct Eco_Type*, unsigned int, struct Eco_Type**);

void Eco_Type_MarkMolecule(struct Eco_GC_State*, struct Eco_Type*, struct Eco_Molecule*);
void Eco_Type_Subclone(struct Eco_CloneState*, struct Eco_Type*, struct Eco_Molecule*, struct Eco_Molecule*);

void Eco_Types_Init();
void Eco_Types_Terminate();

#endif
