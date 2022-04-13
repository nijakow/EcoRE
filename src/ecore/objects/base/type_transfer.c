#include "type_transfer.h"

void Eco_TypeTransfer(struct Eco_Molecule* molecule, struct Eco_Type* new_type)
{
    Eco_Molecule_SwitchType(molecule, new_type);
}
