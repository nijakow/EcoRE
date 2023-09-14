#include "clone.h"

#include <ecore/objects/base/object.h>
#include <ecore/vm/vm.h>
#include <ecore/vm/core/clone.h>
#include <ecore/vm/fiber/fiber.h>
#include <ecore/vm/fiber/stackops.h>
#include <ecore/vm/memory/weak.h>


bool Eco_VM_Builtin_Clone(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_CloneState  state;
    Eco_Any                any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    
    any = Eco_Fiber_Pop(fiber);

    if (Eco_Any_IsPointer(any)) {
        Eco_CloneState_Create(&state, Eco_Any_AsPointer(any));
        Eco_CloneState_CloneAny(&state, &any, &any, true);
        Eco_CloneState_Destroy(&state);
    }

    Eco_Fiber_SetAccu(fiber, any);

    return true;
}

bool Eco_VM_Builtin_EnableFinalization(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_Object*  object;
    Eco_Any             any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    
    any = Eco_Fiber_Pop(fiber);

    if (Eco_Any_IsPointer(any)) {
        object = Eco_Any_AsPointer(any);
        object->bits.wants_finalization = true;
    }

    Eco_Fiber_SetAccu(fiber, any);

    return true;
}

bool Eco_VM_Builtin_NextFinalizableOr(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any             default_value;
    struct Eco_Object*  object;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    default_value = Eco_Fiber_Pop(fiber);
    if (Eco_WeakObjectManager_RemoveObject(&fiber->vm->weak_objects, &object)) {
        Eco_Fiber_SetAccu(fiber, Eco_Any_FromPointer(object));
    } else {
        Eco_Fiber_SetAccu(fiber, default_value);
    }
    return true;
}
