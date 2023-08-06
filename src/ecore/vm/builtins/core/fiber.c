#include "fiber.h"

#include <ecore/objects/vm/code/closure.h>
#include <ecore/vm/core/interpreter.h>
#include <ecore/vm/core/frame.h>
#include <ecore/vm/fiber/fiber.h>
#include <ecore/vm/fiber/stackops.h>

bool Eco_VM_Builtin_StartFiber(struct Eco_Fiber* fiber, unsigned int args)
{
    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    return true;
}
