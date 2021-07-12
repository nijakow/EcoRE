#include <stdio.h>

#include "builtins.h"

#include "builtins_common_header.h"
#include "id.h"
#include "flow.h"

#include "../parser.h"

#include "../../../../objects/key.h"


bool Eco_EConnect_Builtin_Hello(struct Eco_EConnect_Reader* reader,
                                struct Eco_EConnect_Result* result)
{
    printf("Hello :)\n");
    return true;
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
    Eco_EConnect_InstallCallback("ecosphere.econnect.newid", Eco_EConnect_Builtin_NewID);
    Eco_EConnect_InstallCallback("ecosphere.econnect.delid", Eco_EConnect_Builtin_DelID);
    Eco_EConnect_InstallCallback("ecosphere.econnect.id", Eco_EConnect_Builtin_GetID);
    Eco_EConnect_InstallCallback("ecosphere.econnect.block", Eco_EConnect_Builtin_Block);
    Eco_EConnect_InstallCallback("ecosphere.econnect.unwind", Eco_EConnect_Builtin_Unwind);
    Eco_EConnect_InstallCallback("ecosphere.object.key", Eco_EConnect_Builtin_GetKey);
}

void Eco_EConnect_TerminateReaderBuiltins()
{
}
