#include "init.h"

#include <ecore/objects/base/type.h>

extern struct Eco_Object*  Eco_VM_Builtin_LOBBY;
extern struct Eco_Object*  Eco_Integer_PROXY;

extern struct Eco_Type*    Eco_Closure_TYPE;
extern struct Eco_Type*    Eco_Vector_TYPE;


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

bool Eco_VM_Builtin_SetProxy(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  proxy;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    Eco_Fiber_Pop(fiber, &proxy);
    ((struct Eco_Type*) Eco_Any_AsPointer(Eco_Fiber_Peek(fiber)))->proxy = Eco_Any_AsPointer(&proxy);
    return true;
}

bool Eco_VM_Builtin_GetBlockType(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, 0))
        return false;
    Eco_Any_AssignPointer(&value, (struct Eco_Object*) Eco_Closure_TYPE);
    Eco_Fiber_Push(fiber, &value);
    return true;
}

/*bool Eco_VM_Builtin_GetIntegerType(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, 0))
        return false;
    Eco_Any_AssignPointer(&value, (struct Eco_Object*) Eco_Integer_TYPE);
    Eco_Fiber_Push(fiber, &value);
    return true;
}*/

bool Eco_VM_Builtin_GetVectorType(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, 0))
        return false;
    Eco_Any_AssignPointer(&value, (struct Eco_Object*) Eco_Vector_TYPE);
    Eco_Fiber_Push(fiber, &value);
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
