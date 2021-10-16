#include "slots.h"

#include <ecore/objects/base/object_slots.h>


/*
 * TODO, FIXME, XXX!
 *
 * This function has multiple weaknesses:
 *   - It doesn't check for the argument types
 *   - AddSlot might trigger a GC that collects the arguments before they can be used
 *   - There is no way to set the slot flags (e.g. inheritance)
 *   - The return value is set in an inefficient way
 */
bool Eco_VM_Builtin_AddValueSlot(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any                     object;
    Eco_Any                     index;
    Eco_Any                     key;
    Eco_Any                     value;
    struct Eco_Object_SlotInfo  info;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 4, 4))
        return false;
    
    Eco_Fiber_Pop(fiber, &value);
    Eco_Fiber_Pop(fiber, &key);
    Eco_Fiber_Pop(fiber, &index);
    Eco_Fiber_Pop(fiber, &object);

    info.is_inherited = false;
    info.key          = Eco_Any_AsPointer(&key);

    Eco_Object_AddSlot(Eco_Any_AsPointer(&object),
                       Eco_Any_AsInteger(&index),
                       info,
                       &value);

    Eco_Fiber_Push(fiber, &object);

    return true;
}
