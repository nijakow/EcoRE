#include "builtins.h"

#include "objects/key.h"
#include "objects/string.h"
#include "objects/array.h"
#include "objects/ints.h"
#include "objects/object.h"
#include "objects/code.h"
#include "protocol/protocol.h"
#include "id/id.h"
#include "flow/flow.h"

#include <ecore/io/econnect/reader/callback.h>


void Eco_EConnect_InitReaderBuiltins()
{
    Eco_EConnect_InstallCallback("ecosphere.econnect.hello", Eco_EConnect_Builtin_Hello);
    Eco_EConnect_InstallCallback("ecosphere.econnect.newid", Eco_EConnect_Builtin_NewID);
    Eco_EConnect_InstallCallback("ecosphere.econnect.delid", Eco_EConnect_Builtin_DelID);
    Eco_EConnect_InstallCallback("ecosphere.econnect.id", Eco_EConnect_Builtin_GetID);
    Eco_EConnect_InstallCallback("ecosphere.econnect.block", Eco_EConnect_Builtin_Block);
    Eco_EConnect_InstallCallback("ecosphere.econnect.unwind", Eco_EConnect_Builtin_Unwind);
    Eco_EConnect_InstallCallback("ecosphere.object.key", Eco_EConnect_Builtin_GetKey);
    Eco_EConnect_InstallCallback("ecosphere.object.string", Eco_EConnect_Builtin_GetString);
    Eco_EConnect_InstallCallback("ecosphere.object.array", Eco_EConnect_Builtin_GetArray);
    Eco_EConnect_InstallCallback("ecosphere.object.uint", Eco_EConnect_Builtin_GetUInt);
    Eco_EConnect_InstallCallback("ecosphere.object.int", Eco_EConnect_Builtin_GetInt);
    Eco_EConnect_InstallCallback("ecosphere.object.char", Eco_EConnect_Builtin_GetCharacter);
    Eco_EConnect_InstallCallback("ecosphere.object.object", Eco_EConnect_Builtin_GetMolecule);
    Eco_EConnect_InstallCallback("ecosphere.object.molecule", Eco_EConnect_Builtin_GetMolecule);
    Eco_EConnect_InstallCallback("ecosphere.object.code", Eco_EConnect_Builtin_GetCode);
}

void Eco_EConnect_TerminateReaderBuiltins()
{
}
