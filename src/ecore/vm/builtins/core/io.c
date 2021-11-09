#include "io.h"

#include <ecore/io/logging/log.h>


bool Eco_VM_Builtin_Print(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, ECO_VM_BUILTIN_INFINITE_ARGS))
        return false;

    while (args --> 0)
    {
        Eco_Fiber_Pop(fiber, &any);


        if (Eco_Any_IsInteger(&any)) {
            Eco_Log_Info("Integer: %d\n", Eco_Any_AsInteger(&any));
        } else if (Eco_Any_IsFloating(&any)) {
            Eco_Log_Info("Floating: %f\n", Eco_Any_AsFloating(&any));
        } else if (Eco_Any_IsPointer(&any)) {
            Eco_Log_Info("Pointer: %p\n", Eco_Any_AsPointer(&any));
        } else if (Eco_Any_IsCharacter(&any)) {
            Eco_Log_Info("Character: %-10d #\\%c\n", Eco_Any_AsCharacter(&any), Eco_Any_AsCharacter(&any));
        } else {
            Eco_Log_Info("???\n");
        }
    }

    Eco_Fiber_Push(fiber, &any);

    return true;
}
