#include "key.h"

#include "../../parser.h"

#include "../../../../../objects/key.h"


bool Eco_EConnect_Builtin_GetKey(struct Eco_EConnect_Reader* reader,
                                 struct Eco_EConnect_Result* result)
{
    unsigned int     id;
    struct Eco_Key*  key;

    id  = Eco_EConnect_ParseUInt(&reader->stream);
    key = Eco_EConnect_ParseKey(&reader->stream);

    if (key != NULL) {
        Eco_EConnect_Instance_OptionallyBindObject(reader->instance, (struct Eco_Object*) key, id);
        Eco_EConnect_Result_Create_Object(result, (struct Eco_Object*) key);
    } else {
        Eco_EConnect_Result_Create_Error(result, Eco_EConnect_ErrorType_KEY_NOT_FOUND);
    }

    return true;
}
