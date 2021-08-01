#include "io.h"

#include "../../../io/logging/log.h"


bool Eco_VM_Builtin_Print(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1)) return false;

    Eco_Any_AssignAny(&any, Eco_Fiber_Nth(fiber, 1));

    if (Eco_Any_IsInteger(&any)) {
        Eco_Log_Info("Integer: %d\n", Eco_Any_AsInteger(&any));
    } else if (Eco_Any_IsFloating(&any)) {
        Eco_Log_Info("Floating: %f\n", Eco_Any_AsFloating(&any));
    } else if (Eco_Any_IsPointer(&any)) {
        Eco_Log_Info("Pointer: %p\n", Eco_Any_AsPointer(&any));
    } else {
        Eco_Log_Info("???\n");
    }

    return true;
}
