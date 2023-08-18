
#include "molecule.h"

#include <ecore/objects/base/object.h>
#include <ecore/objects/base/type.h>
#include <ecore/objects/base/type/gc.h>
#include <ecore/vm/core/clone.h>
#include <ecore/vm/core/send.h>


static struct Eco_TypeCore Eco_Molecule_TYPECORE;
static struct Eco_Type*    Eco_Molecule_TYPE;


void Eco_Molecule_UnlinkFromTypeList(struct Eco_Molecule* molecule)
{
    struct Eco_Type*       type;
    struct Eco_Molecule**  ptr;

    type =  molecule->_.type;
    ptr  = &type->implementing_molecules;

    while (*ptr != NULL)
    {
        if (*ptr == molecule) {
            *ptr = molecule->next_of_same_type;
            break;
        }
        ptr = &((*ptr)->next_of_same_type);
    }
}

void Eco_Molecule_LinkIntoTypeList(struct Eco_Molecule* molecule, struct Eco_Type* type)
{
    molecule->next_of_same_type  = type->implementing_molecules;
    type->implementing_molecules = molecule;
}


struct Eco_Molecule* Eco_Molecule_New(struct Eco_Type* type)
{
    struct Eco_Molecule*  molecule;

    molecule          = Eco_Object_New(type, sizeof(struct Eco_Molecule));

    if (molecule != NULL)
    {
        molecule->payload = Eco_Object_Payload_New(type->instance_payload_size);
        Eco_Molecule_LinkIntoTypeList(molecule, type);
    }

    return molecule;
}

struct Eco_Molecule* Eco_Molecule_NewPlain()
{
    return Eco_Molecule_New(Eco_Molecule_TYPE);
}

void Eco_Molecule_Del(struct Eco_Molecule* molecule)
{
    Eco_Molecule_UnlinkFromTypeList(molecule);
    if (molecule->payload != NULL)
        Eco_Object_Payload_Delete(molecule->payload);
    Eco_Object_Del(&molecule->_);
}


void Eco_Molecule_MarkChildren(struct Eco_GC_State* state, struct Eco_Molecule* molecule)
{
    Eco_Type_MarkMolecule(state, molecule->_.type, molecule);
    Eco_Object_MarkChildren(state, &molecule->_);
}

void Eco_Molecule_MarkInstance(struct Eco_GC_State* state, struct Eco_Molecule* molecule)
{
    Eco_Object_MarkInstance(state, &molecule->_);
}



struct Eco_Molecule* Eco_Molecule_Clone(struct Eco_CloneState* state,
                                        struct Eco_Molecule* molecule,
                                        bool forced)
{
    /*
     * Clone a molecule.
     * 
     * The cloning process is quite complex. There are multiple
     * factors that influence whether the molecule is returned as-is
     * or if an actual copy is going to be allocated.
     * 
     * The rules are:
     *   - If the object was already cloned before, we always return
     *     the existing clone. This is important in cases where a
     *     subcomponent (e.g. a member of a module that's being cloned)
     *     references back to its parent with a normal ('own' bit not set)
     *     slot. Since the module has already been cloned, the pointer to
     *     it will be updated.
     *   - If the object's UP object has been cloned, we clone this object
     *     too, since cloning of groups will always imply cloning of children.
     *   - If the object cloning was "forced" (e.g. if the cloning primitive
     *     was invoked directly on this object or if this object is the value
     *     of a slot marked with the 'own' bit), we also clone this object.
     *   - Otherwise, we don't clone this object.
     * 
     *                                                - nijakow
     * 
     * TODO: These rules must be followed for all objects. Moving this code
     *       to src/ecore/vm/core/clone.c might be a better solution.
     *                                                - nijakow
     */
    struct Eco_Object*    up;
    struct Eco_Molecule*  clone;

    clone = (struct Eco_Molecule*) Eco_CloneState_QueryClone(state, (struct Eco_Object*) molecule);
    if (clone == NULL)
    {
        up = NULL;

        if (up == NULL && !forced)
            return molecule;

        clone = Eco_Molecule_New(molecule->_.type);
        Eco_CloneState_RegisterClone(state, (struct Eco_Object*) molecule, (struct Eco_Object*) clone);
        Eco_Type_Subclone(state, molecule->_.type, molecule, clone);
    }

    return clone;
}


bool Eco_Molecule_IsMolecule(struct Eco_Object* object)
{
    return object->type->typecore == &Eco_Molecule_TYPECORE;
}


void Eco_Molecule_Init()
{
    Eco_TypeCore_Create(&Eco_Molecule_TYPECORE, "Eco_Molecule");
    
    Eco_Molecule_TYPECORE.send          = (Eco_TypeCore_SendFunc)  Eco_Object_Send;
    Eco_Molecule_TYPECORE.mark_instance = (Eco_TypeCore_MarkFunc)  Eco_Molecule_MarkInstance;
    Eco_Molecule_TYPECORE.mark_children = (Eco_TypeCore_MarkFunc)  Eco_Molecule_MarkChildren;
    Eco_Molecule_TYPECORE.clone         = (Eco_TypeCore_CloneFunc) Eco_Molecule_Clone;
    Eco_Molecule_TYPECORE.del           = (Eco_TypeCore_DelFunc)   Eco_Molecule_Del;

    Eco_Molecule_TYPE                   = Eco_Type_NewPrefab(&Eco_Molecule_TYPECORE);
}

void Eco_Molecule_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_Molecule_TYPECORE);
}
