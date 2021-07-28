#include "builtins.h"

#include "builtin.h"

#include "../fiber.h"
#include "../../objects/misc/key/key.h"
#include "../../io/logging/log.h"


struct Eco_Object* Eco_VM_Builtin_LOBBY = NULL;


bool Eco_VM_Builtin_Trap(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_BUILTIN_TRAP);
    return false;
}

static bool Eco_VM_Builtin_Hello(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Log_Info("Hello :)\n");
    return true;
}


static void Eco_VM_Builtins_AddBuiltin(const char* name, Eco_Builtin builtin)
{
    struct Eco_Key*  key;

    key = Eco_Key_Find(name);
    
    if (key != NULL) {
        key->builtin = builtin;
    } else {
        Eco_Log_Critical("Unable to install builtin '%s': Key search returned NULL!\n");
    }
}


void Eco_VM_Builtins_Init()
{
    Eco_VM_Builtin_LOBBY = Eco_Object_New();

    Eco_VM_Builtins_AddBuiltin("hello", Eco_VM_Builtin_Hello);
}

void Eco_VM_Builtins_Terminate()
{
}
