#include "object.h"

#include "object_body.h"

#include <ecore/io/econnect/reader/parser.h>
#include <ecore/objects/base/molecule.h>


bool Eco_EConnect_Builtin_GetMolecule(struct Eco_EConnect_Reader* reader,
                                      struct Eco_EConnect_Result* result)
{
    unsigned int          id;
    struct Eco_Molecule*  the_object;

    id         = Eco_EConnect_ParseUInt(&reader->stream);
    the_object = Eco_Molecule_NewPlain();

    Eco_EConnect_Instance_OptionallyBindObject(reader->instance, (struct Eco_Object*) the_object, id);
    if (!Eco_EConnect_Reader_ReadMoleculeBody(reader, result, the_object))
        return false;
    Eco_EConnect_Result_Create_Object(result, (struct Eco_Object*) the_object);

    return true;
}
