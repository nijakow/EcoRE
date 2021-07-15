#include "builtins.h"

#include "objects/key.h"
#include "objects/object.h"
#include "protocol/protocol.h"
#include "id/id.h"
#include "flow/flow.h"

#include "../callback.h"


void Eco_EConnect_InitReaderBuiltins()
{
    Eco_EConnect_InstallCallback("ecosphere.econnect.hello", Eco_EConnect_Builtin_Hello);
    Eco_EConnect_InstallCallback("ecosphere.econnect.newid", Eco_EConnect_Builtin_NewID);
    Eco_EConnect_InstallCallback("ecosphere.econnect.delid", Eco_EConnect_Builtin_DelID);
    Eco_EConnect_InstallCallback("ecosphere.econnect.id", Eco_EConnect_Builtin_GetID);
    Eco_EConnect_InstallCallback("ecosphere.econnect.block", Eco_EConnect_Builtin_Block);
    Eco_EConnect_InstallCallback("ecosphere.econnect.unwind", Eco_EConnect_Builtin_Unwind);
    Eco_EConnect_InstallCallback("ecosphere.object.key", Eco_EConnect_Builtin_GetKey);
    Eco_EConnect_InstallCallback("ecosphere.object.object", Eco_EConnect_Builtin_GetObject);
    Eco_EConnect_InstallCallback("ecosphere.object.code", Eco_EConnect_Builtin_GetObject);
}

void Eco_EConnect_TerminateReaderBuiltins()
{
}
