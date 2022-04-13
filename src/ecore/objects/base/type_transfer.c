#include <ecore/objects/base/type/copying.h>

#include "type_transfer.h"


void Eco_TypeTransfer(struct Eco_Molecule* molecule,
                      unsigned int         slot_index,
                      struct Eco_Type*     referenced_type)
{
    struct Eco_Type*  new_type;

    if (Eco_Type_CopyWithChangedSlotTypeReference(molecule->_.type, slot_index, referenced_type, &new_type))
    {
        Eco_Molecule_SwitchType(molecule, new_type);
    }
    // TODO, FIXME, XXX: Else?
}
