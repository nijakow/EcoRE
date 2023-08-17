#include "clone.h"

#include <ecore/vm/core/clone.h>
#include <ecore/vm/fiber/fiber.h>
#include <ecore/vm/fiber/stackops.h>


bool Eco_VM_Builtin_Clone(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_CloneState  state;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    if (Eco_Any_IsPointer(*Eco_Fiber_Peek(fiber))) {
        Eco_CloneState_Create(&state, Eco_Any_AsPointer(*Eco_Fiber_Peek(fiber)));
        Eco_CloneState_CloneAny(&state, Eco_Fiber_Peek(fiber), Eco_Fiber_Peek(fiber), true);
        Eco_CloneState_Destroy(&state);
    }
    return true;
}
