#ifndef ECO_VM_BUILTINS_BUILTIN_H
#define ECO_VM_BUILTINS_BUILTIN_H

#include "../../eco.h"

#include "../fiber.h"


typedef bool (*Eco_Builtin)(struct Eco_Fiber*, unsigned int);

bool Eco_VM_Builtin_Trap(struct Eco_Fiber*, unsigned int);


#define ECO_VM_BUILTIN_INFINITE_ARGS 0xffffffff

static inline bool Eco_VM_Builtin_Tool_ArgExpect(struct Eco_Fiber* fiber,
                                   unsigned int args,
                                   unsigned int minargs,
                                   unsigned int maxargs)
{
    if (args < minargs || (args > maxargs)) {
        Eco_Fiber_SetState(fiber, Eco_Fiber_State_ERROR_ARGERROR);
        return false;
    } else {
        return true;
    }
}

#endif
