#include "gc.h"

#include <ecore/objects/base/molecule/molecule.h>
#include <ecore/vm/memory/gc/gc.h>


void Eco_Type_Mark(struct Eco_GC_State* state, struct Eco_Type* type)
{
    unsigned int  i;

    for (i = 0; i < type->slot_count; i++)
    {
        Eco_GC_State_MarkObject(state, type->slots[i].info.key);
        Eco_GC_State_MarkObject(state, (struct Eco_Object*) type->slots[i].interface);
        switch (type->slots[i].type)
        {
            case Eco_TypeSlotType_INLINED:
                if (type->slots[i].body.inlined.referenced_type != NULL)
                    Eco_GC_State_MarkObject(state, type->slots[i].body.inlined.referenced_type);
                break;
            case Eco_TypeSlotType_CODE:
                Eco_GC_State_MarkObject(state, type->slots[i].body.code.code);
                break;
        }
    }
    if (type->proxy != NULL)
        Eco_GC_State_MarkObject(state, type->proxy);
    if (type->interface != NULL)
        Eco_GC_State_MarkObject(state, type->interface);
    if (type->public_interface != NULL)
        Eco_GC_State_MarkObject(state, type->public_interface);
    Eco_Object_Mark(state, &type->_);
}

void Eco_Type_MarkMolecule(struct Eco_GC_State* state,
                           struct Eco_Type* type,
                           struct Eco_Molecule* molecule)
{
    unsigned int  i;

    for (i = 0; i < type->slot_count; i++)
    {
        switch (type->slots[i].type)
        {
            case Eco_TypeSlotType_INLINED:
                Eco_GC_State_MarkAny(state, *((Eco_Any*) Eco_Molecule_At(molecule, type->slots[i].body.inlined.offset)));
                break;
            default:
                break;
        }
    }
}
