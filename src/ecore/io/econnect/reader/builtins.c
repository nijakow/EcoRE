#include <stdio.h>

#include "builtins.h"

#include "callback.h"
#include "reader.h"
#include "result.h"
#include "parser.h"

#include "../../../objects/key.h"


bool Eco_EConnect_Builtin_Hello(struct Eco_EConnect_Reader* reader,
                                struct Eco_EConnect_Result* result)
{
    printf("Hello :)\n");
    return true;
}

bool Eco_EConnect_Builtin_Bind(struct Eco_EConnect_Reader* reader,
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
            Eco_EConnect_Result_Create_Error(result, Eco_EConnect_ErrorType_EXPECTED_OBJECT);
        }
    }

    return false;
}

bool Eco_EConnect_Builtin_Unbind(struct Eco_EConnect_Reader* reader,
                                 struct Eco_EConnect_Result* result)
{
    unsigned int  id;

    id = Eco_EConnect_ParseUInt(&reader->stream);

    Eco_EConnect_Instance_UnbindObject(reader->instance, id);
    Eco_EConnect_Result_Create_None(result);

    return true;
}

bool Eco_EConnect_Builtin_Block(struct Eco_EConnect_Reader* reader,
                                struct Eco_EConnect_Result* result)
{
    unsigned int  elements;
    unsigned int  i;

    elements = Eco_EConnect_ParseUInt(&reader->stream);

    if (elements == 0) {
        Eco_EConnect_Result_Create_None(result);
        return true;
    }

    for (i = 0; i < elements - 1; i++)
    {
        if (Eco_EConnect_Reader_Read(reader, result)) {
            Eco_EConnect_Result_Destroy(result);
        } else {
            return false;
        }
    }

    return Eco_EConnect_Reader_Read(reader, result);
}

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


void Eco_EConnect_InitReaderBuiltins()
{
    Eco_EConnect_InstallCallback("ecosphere.econnect.hello", Eco_EConnect_Builtin_Hello);
    Eco_EConnect_InstallCallback("ecosphere.econnect.bind", Eco_EConnect_Builtin_Bind);
    Eco_EConnect_InstallCallback("ecosphere.econnect.unbind", Eco_EConnect_Builtin_Unbind);
    Eco_EConnect_InstallCallback("ecosphere.econnect.block", Eco_EConnect_Builtin_Bind);
    Eco_EConnect_InstallCallback("ecosphere.object.key", Eco_EConnect_Builtin_GetKey);
}

void Eco_EConnect_TerminateReaderBuiltins()
{
}
