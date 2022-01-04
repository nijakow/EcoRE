#include "reflection.h"

#include <ecore/base/extra.h>
#include <ecore/objects/misc/array/array.h>
#include <ecore/objects/vm/interface/interface.h>


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


bool Eco_VM_Builtin_InterfaceGetEntryInfo(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_Interface*  interface;
    Eco_Any                any;
    int                    index;
    int                    subindex;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 3, 3))
        return false;
    
    Eco_Fiber_Pop(fiber, &any);
    subindex = Eco_Any_AsInteger(&any);

    Eco_Fiber_Pop(fiber, &any);
    index = Eco_Any_AsInteger(&any);
    
    Eco_Fiber_Pop(fiber, &any);
    interface = (struct Eco_Interface*) Eco_Any_AsPointer(&any);

    if (index < 0 || ((unsigned int) index) >= interface->entry_count)
        Eco_Any_AssignPointer(&any, (struct Eco_Object*) interface);
    else if (subindex == -1)
        Eco_Any_AssignPointer(&any, (struct Eco_Object*) interface->entries[index].key);
    else if (subindex == -2)
        Eco_Any_AssignPointer(&any, (struct Eco_Object*) interface->entries[index].return_type);
    else if (subindex == -3)
        Eco_Any_AssignInteger(&any, interface->entries[index].arg_count);
    else if (subindex == -4)
        Eco_Any_AssignInteger(&any, interface->entries[index].has_varargs);
    else if (subindex < 0 || ((unsigned int) subindex) >= interface->entries[index].arg_count)
        Eco_Any_AssignPointer(&any, (struct Eco_Object*) interface);
    else
        Eco_Any_AssignPointer(&any, (struct Eco_Object*) interface->entries[index].arg_types[subindex]);
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_InterfaceAddEntry(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_InterfaceEntry  entry;
    struct Eco_Interface*      interface;
    Eco_Any                    any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 4, ECO_VM_BUILTIN_INFINITE_ARGS))
        return false;
    
    entry.arg_count = args - 4;
    
    while (args > 4)
    {
        Eco_Fiber_Pop(fiber, &any);
        entry.arg_types[args - 4] = (struct Eco_Interface*) Eco_Any_AsPointer(&any);
        args--;
    }

    Eco_Fiber_Pop(fiber, &any);
    entry.has_varargs = Eco_Any_AsInteger(&any) != 0;

    Eco_Fiber_Pop(fiber, &any);
    entry.key = (struct Eco_Key*) Eco_Any_AsPointer(&any);

    Eco_Fiber_Pop(fiber, &any);
    entry.return_type = (struct Eco_Interface*) Eco_Any_AsPointer(&any);

    Eco_Fiber_Pop(fiber, &any);
    interface = (struct Eco_Interface*) Eco_Any_AsPointer(&any);

    interface = Eco_Interface_AddEntry(interface, &entry);

    // TODO, FIXME, XXX: Check if interface == NULL!

    Eco_Any_AssignPointer(&any, (struct Eco_Object*) interface);
    Eco_Fiber_Push(fiber, &any);
    return true;
}
