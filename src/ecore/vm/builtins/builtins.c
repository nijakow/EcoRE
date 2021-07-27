#include "builtins.h"

#include "../../objects/base/object.h"
#include "../../objects/base/object_slots.h"
#include "../../objects/base/builtin.h"

#include "../../io/logging/log.h"


struct Eco_Object* Eco_VM_Builtin_LOBBY = NULL;



bool Eco_VM_Builtin_Hello(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Log_Info("Hello :)\n");
    return true;
}


void Eco_VM_Builtins_Init()
{
    Eco_VM_Builtin_LOBBY = Eco_Object_New();

    Eco_Object_AddBuiltin(Eco_VM_Builtin_LOBBY, "hello", Eco_VM_Builtin_Hello);
}

void Eco_VM_Builtins_Terminate()
{
}
