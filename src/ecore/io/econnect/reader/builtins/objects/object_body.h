#ifndef ECO_IO_ECONNECT_READER_BUILTINS_OBJECTS_OBJECT_BODY_H
#define ECO_IO_ECONNECT_READER_BUILTINS_OBJECTS_OBJECT_BODY_H

#include "../builtins_common_header.h"
#include "../../../../../objects/base/object.h"

bool Eco_EConnect_Reader_ReadObjectBody(struct Eco_EConnect_Reader*,
                                        struct Eco_EConnect_Result*,
                                        struct Eco_Object*);

#endif
