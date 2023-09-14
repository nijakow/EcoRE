#include "string.h"

#include <ecore/objects/vm/weakbox/weakbox.h>


bool Eco_VM_Builtin_WeakBoxNew(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_WeakBox*  weakbox;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, 0))
        return false;
    weakbox = Eco_WeakBox_New();
    Eco_Fiber_SetAccu(fiber, Eco_Any_FromPointer(weakbox));
    return true;
}

bool Eco_VM_Builtin_WeakBoxGet(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_WeakBox*  weakbox;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    weakbox = Eco_Any_AsPointer(Eco_Fiber_Pop(fiber));
    Eco_Fiber_SetAccu(fiber, Eco_WeakBox_GetValue(weakbox));
    return true;
}

bool Eco_VM_Builtin_WeakBoxSet(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_WeakBox*  weakbox;
    Eco_Any              value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    value   = Eco_Fiber_Pop(fiber);
    weakbox = Eco_Any_AsPointer(Eco_Fiber_Pop(fiber));
    Eco_WeakBox_SetValue(weakbox, value);
    return true;
}
