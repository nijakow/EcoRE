
#include "molecule.h"

#include <ecore/objects/base/object.h>
#include <ecore/objects/base/type.h>
#include <ecore/vm/core/clone.h>
#include <ecore/vm/core/send.h>


static struct Eco_TypeCore Eco_Molecule_TYPECORE;
static struct Eco_Type*    Eco_Molecule_TYPE;



struct Eco_Molecule* Eco_Molecule_New(struct Eco_Type* type)
{
    struct Eco_Molecule*  molecule;

    molecule          = Eco_Object_New(type, sizeof(struct Eco_Molecule), 0);

    molecule->payload = Eco_Object_Payload_New(type->instance_payload_size);

    return molecule;
}

struct Eco_Molecule* Eco_Molecule_NewPlain()
{
    return Eco_Molecule_New(Eco_Molecule_TYPE);
}

void Eco_Molecule_Del(struct Eco_Molecule* molecule)
{
    if (molecule->payload != NULL)
        Eco_Object_Payload_Delete(molecule->payload);
    Eco_Object_Del(&molecule->_);
}


void Eco_Molecule_Mark(struct Eco_GC_State* state, struct Eco_Molecule* molecule)
{
    Eco_Type_MarkMolecule(state, molecule->_.type, molecule);
    Eco_Object_Mark(state, &molecule->_);
}



struct Eco_Molecule* Eco_Molecule_Clone(struct Eco_CloneState* state, struct Eco_Molecule* molecule)
{
    struct Eco_Molecule*  clone;

    clone = (struct Eco_Molecule*) Eco_CloneState_QueryClone(state, (struct Eco_Object*) molecule);

    if (clone == NULL) {
        clone = Eco_Molecule_New(molecule->_.type);
        Eco_CloneState_RegisterClone(state, (struct Eco_Object*) molecule, (struct Eco_Object*) clone);
        Eco_Type_Subclone(state, molecule->_.type, molecule, clone);
    }

    return clone;
}



void Eco_Molecule_Init()
{
    Eco_TypeCore_Create(&Eco_Molecule_TYPECORE, "Eco_Molecule");
    
    Eco_Molecule_TYPECORE.send  = (Eco_TypeCore_SendFunc)  Eco_Object_Send;
    Eco_Molecule_TYPECORE.mark  = (Eco_TypeCore_MarkFunc)  Eco_Molecule_Mark;
    Eco_Molecule_TYPECORE.clone = (Eco_TypeCore_CloneFunc) Eco_Molecule_Clone;
    Eco_Molecule_TYPECORE.del   = (Eco_TypeCore_DelFunc)   Eco_Molecule_Del;

    Eco_Molecule_TYPE           = Eco_Type_NewPrefab(&Eco_Molecule_TYPECORE);
}

void Eco_Molecule_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_Molecule_TYPECORE);
}
