#include "econnect.h"

#include "reader/callback.h"
#include "reader/reader.h"
#include "reader/parser.h"
#include "reader/result.h"

#include "../../eco.h"


bool Eco_EConnect_Builtin_GetKey(struct Eco_EConnect_Reader* reader,
                                 struct Eco_EConnect_Result* result)
{
    struct Eco_Key*  key;

    key = Eco_EConnect_ParseKey(&(reader->stream));

    if (key != NULL) {
        Eco_EConnect_Result_Create_Object(result, (struct Eco_Object*) key);
    } else {
        Eco_EConnect_Result_Create_Error(result, Eco_EConnect_ErrorType_KEY_NOT_FOUND);
    }

    return true;
}


void Eco_EConnect_Init()
{
    Eco_EConnect_InstallCallback("ecosphere.object.key", Eco_EConnect_Builtin_GetKey);
}

void Eco_EConnect_Terminate()
{
}
