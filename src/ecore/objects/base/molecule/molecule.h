#ifndef ECO_OBJECTS_MOLECULE_MOLECULE_H
#define ECO_OBJECTS_MOLECULE_MOLECULE_H

#include <ecore/objects/base/object.h>
#include <ecore/objects/base/molecule/object_payload.h>


struct Eco_Molecule
{
    struct Eco_Object           _;
    struct Eco_Molecule*        next_of_same_type;
    struct Eco_Object_Payload*  payload;
};

static inline void* Eco_Molecule_At(struct Eco_Molecule* object, unsigned int offset)
{
    /* TODO, FIXME, XXX: Add a debug setting: If out-of-bounds, return NULL! */
    return (void*) &(object->payload->data[offset]);
}


struct Eco_Molecule* Eco_Molecule_NewPlain();

void Eco_Molecule_UnlinkFromTypeList(struct Eco_Molecule*);
void Eco_Molecule_LinkIntoTypeList(struct Eco_Molecule*, struct Eco_Type*);
void Eco_Molecule_SwitchType(struct Eco_Molecule*, struct Eco_Type*);

bool Eco_Molecule_IsMolecule(struct Eco_Object*);

void Eco_Molecule_Init();
void Eco_Molecule_Terminate();

#endif
