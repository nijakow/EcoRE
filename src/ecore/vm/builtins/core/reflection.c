#include "reflection.h"

#include <ecore/base/extra.h>
#include <ecore/objects/misc/array/array.h>

bool Eco_VM_Builtin_GetType(struct Eco_Fiber* fiber, unsigned int args)
{
    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Any_AssignPointer(Eco_Fiber_Peek(fiber),
                          (struct Eco_Object*) Eco_Any_GetType(Eco_Fiber_Peek(fiber)));
    return true;
}

bool Eco_VM_Builtin_GetTypeSlotNames(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any            any;
    struct Eco_Array*  array;
    struct Eco_Type*   type;
    unsigned int       index;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Fiber_Pop(fiber, &any);
    type = (struct Eco_Type*) Eco_Any_AsPointer(&any);
    array = Eco_Array_New(type->slot_count);
    // TODO, FIXME, XXX: Handle array == NULL!
    for (index = 0; index < type->slot_count; index++)
    {
        Eco_Any_AssignPointer(&any, type->slots[index].key);
        Eco_Array_Put(array, index, &any);
    }
    Eco_Any_AssignPointer(&any, (struct Eco_Object*) array);
    Eco_Fiber_Push(fiber, &any);
    return true;
}
