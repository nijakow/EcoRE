#include "builtins.h"

#include "protocol.h"
#include "id.h"
#include "flow.h"
#include "objects/key.h"

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
}

void Eco_EConnect_TerminateReaderBuiltins()
{
}
