#include <ecore/base/extra.h>
#include <ecore/objects/base/type/copying.h>

#include "type_transfer.h"

static void Eco_TypeTransfer1(struct Eco_Molecule*, unsigned int, struct Eco_Type*);

extern struct Eco_Type*  Eco_ALL_TYPES;


static void Eco_TypeTransfer_Patch()
{
    struct Eco_Type*              type;
    struct Eco_Type*              referenced_type;
    struct Eco_Molecule*          molecule;
    unsigned int                  slot;
    Eco_Any                       any;

  again:
    for (type = Eco_ALL_TYPES; type != NULL; type = type->next)
    {
        for (molecule = type->implementing_molecules;
                molecule != NULL;
                molecule = molecule->next_of_same_type)
        {
            for (slot = 0; slot < type->slot_count; slot++)
            {
                if (type->slots[slot].type == Eco_TypeSlotType_INLINED && type->slots[slot].body.inlined.referenced_type != NULL)
                {
                    Eco_TypeSlot_GetValue(&type->slots[slot], molecule, &any);  // TODO: Return value
                    referenced_type = Eco_Any_GetType(any);
                    if (referenced_type != type->slots[slot].body.inlined.referenced_type)
                    {
                        Eco_TypeTransfer1(molecule, slot, referenced_type);
                        goto again;
                    }
                }
            }
        }
    }
}

static void Eco_TypeTransfer1(struct Eco_Molecule* molecule,
                              unsigned int         slot_index,
                              struct Eco_Type*     referenced_type)
{
    struct Eco_Type*  new_type;

    if (Eco_Type_CopyWithChangedSlotTypeReference(molecule->_.type, molecule, slot_index, referenced_type, &new_type))
    {
        Eco_Molecule_SwitchType(molecule, new_type);
    }
    // TODO, FIXME, XXX: Else?
}

void Eco_TypeTransfer(struct Eco_Molecule* molecule,
                      unsigned int         slot_index,
                      struct Eco_Type*     referenced_type)
{
    if (molecule->_.type->slots[slot_index].body.inlined.referenced_type != referenced_type)
    {
        Eco_TypeTransfer1(molecule, slot_index, referenced_type);
        Eco_TypeTransfer_Patch();
    }
}
