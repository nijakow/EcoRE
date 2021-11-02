#include "init.h"

extern struct Eco_Object*  Eco_VM_Builtin_LOBBY;
extern struct Eco_Object*  Eco_Closure_PROXY;
extern struct Eco_Object*  Eco_Integer_PROXY;

bool Eco_VM_Builtin_SetLobby(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Any_AssignAny(&any, Eco_Fiber_Peek(fiber));
    if (Eco_Any_IsPointer(&any))
        Eco_VM_Builtin_LOBBY = Eco_Any_AsPointer(&any);
    else
        return false;
    return true;
}

bool Eco_VM_Builtin_SetBlockProxy(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Any_AssignAny(&any, Eco_Fiber_Peek(fiber));
    if (Eco_Any_IsPointer(&any))
        Eco_Closure_PROXY = Eco_Any_AsPointer(&any);
    else
        return false;
    return true;
}

bool Eco_VM_Builtin_SetIntegerProxy(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Any_AssignAny(&any, Eco_Fiber_Peek(fiber));
    if (Eco_Any_IsPointer(&any))
        Eco_Integer_PROXY = Eco_Any_AsPointer(&any);
    else
        return false;
    return true;
}
