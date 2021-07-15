#include "id.h"

#include "../parser.h"


bool Eco_EConnect_Builtin_NewID(struct Eco_EConnect_Reader* reader,
                                struct Eco_EConnect_Result* result)
{
    unsigned int        id;
    struct Eco_Object*  object;

    id = Eco_EConnect_ParseUInt(&reader->stream);

    if (Eco_EConnect_Reader_Read(reader, result)) {
        if (Eco_EConnect_Result_ExpectObject(result, &object)) {
            Eco_EConnect_Instance_BindObject(reader->instance, object, id);
            Eco_EConnect_Result_Destroy(result);
            return Eco_EConnect_Reader_Read(reader, result);
        } else {
            Eco_EConnect_Result_Destroy(result);
            Eco_EConnect_Result_Create_Error(result, Eco_EConnect_ErrorType_TYPE_ERROR);
        }
    }

    return false;
}

bool Eco_EConnect_Builtin_DelID(struct Eco_EConnect_Reader* reader,
                                struct Eco_EConnect_Result* result)
{
    unsigned int  id;

    id = Eco_EConnect_ParseUInt(&reader->stream);

    Eco_EConnect_Instance_UnbindObject(reader->instance, id);
    Eco_EConnect_Result_Create_None(result);

    return true;
}

bool Eco_EConnect_Builtin_GetID(struct Eco_EConnect_Reader* reader,
                                struct Eco_EConnect_Result* result)
{
    struct Eco_Object*  object;

    object = Eco_EConnect_Reader_ReadObjectByID(reader);

    if (object != NULL) {
        Eco_EConnect_Result_Create_Object(result, object);
        return true;
    } else {
        Eco_EConnect_Result_Create_Error(result, Eco_EConnect_ErrorType_UNBOUND_OBJECT);
        return false;
    }
}
