#ifndef ECO_IO_ECONNECT_READER_BUILTINS_OBJECTS_OBJECT_BODY_H
#define ECO_IO_ECONNECT_READER_BUILTINS_OBJECTS_OBJECT_BODY_H

#include <ecore/io/econnect/reader/builtins/builtins_common_header.h>
#include <ecore/objects/base/molecule/molecule.h>

bool Eco_EConnect_Reader_ReadMoleculeBody(struct Eco_EConnect_Reader*,
                                          struct Eco_EConnect_Result*,
                                          struct Eco_Molecule*);

#endif
