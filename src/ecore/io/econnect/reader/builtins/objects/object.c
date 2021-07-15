#include "object.h"

#include "object_body.h"

#include "../../parser.h"
#include "../../../../../objects/base/object.h"


bool Eco_EConnect_Builtin_GetObject(struct Eco_EConnect_Reader* reader,
                                    struct Eco_EConnect_Result* result)
{
    unsigned int        id;
    struct Eco_Object*  the_object;

    id         = Eco_EConnect_ParseUInt(&reader->stream);
    the_object = Eco_Object_New();

    Eco_EConnect_Instance_OptionallyBindObject(reader->instance, the_object, id);
    if (!Eco_EConnect_Reader_ReadObjectBody(reader, result, the_object))
        return false;
    Eco_EConnect_Result_Create_Object(result, the_object);

    return true;
}
