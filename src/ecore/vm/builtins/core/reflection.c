#include "reflection.h"

#include <ecore/base/extra.h>
#include <ecore/objects/misc/array/array.h>
#include <ecore/objects/vm/interface/interface.h>
#include <ecore/vm/vm.h>

bool Eco_VM_Builtin_GetType(struct Eco_Fiber* fiber, unsigned int args)
{
    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    *Eco_Fiber_Peek(fiber) = Eco_Any_FromPointer(Eco_Any_GetType(Eco_Fiber_Peek(fiber)));
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
    type = (struct Eco_Type*) Eco_Any_AsPointer(any);
    array = Eco_Array_New(type->slot_count);
    // TODO, FIXME, XXX: Handle array == NULL!
    for (index = 0; index < type->slot_count; index++)
    {
        any = Eco_Any_FromPointer(type->slots[index].info.key);
        Eco_Array_Put(array, index, &any);
    }
    any = Eco_Any_FromPointer(array);
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_GetTypeSlotInfo(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_Type*  type;
    Eco_Any           any;
    int               index;
    int               subindex;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 3, 3))
        return false;
    
    Eco_Fiber_Pop(fiber, &any);
    subindex = Eco_Any_AsInteger(any);

    Eco_Fiber_Pop(fiber, &any);
    index = Eco_Any_AsInteger(any);
    
    Eco_Fiber_Pop(fiber, &any);
    type = Eco_Any_AsPointer(any);

    if (subindex == -1)
        any = Eco_Any_FromInteger(type->slot_count);
    else if (subindex == -2)
        any = Eco_Any_FromPointer(type->slots[index].info.key);
    else if (subindex == -3) {
        if (type->slots[index].interface == NULL)
            any = Eco_Any_FromPointer(Eco_Interface_GetDefaultInterface());
        else
            any = Eco_Any_FromPointer(type->slots[index].interface);
    }
    else if (subindex == -4) {
        if (type->slots->type == Eco_TypeSlotType_CODE) {
            any = Eco_Any_FromPointer(type->slots[index].body.code.code);
        } else if (type->slots->type == Eco_TypeSlotType_INLINED) {
            any = Eco_Any_FromInteger(type->slots[index].body.inlined.offset);
        } else {
            Eco_Any_AssignAny(&any, &fiber->vm->constants.cfalse);
        }
    }
    else
        Eco_Any_AssignAny(&any, &fiber->vm->constants.cfalse);
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
    subindex = Eco_Any_AsInteger(any);

    Eco_Fiber_Pop(fiber, &any);
    index = Eco_Any_AsInteger(any);
    
    Eco_Fiber_Pop(fiber, &any);
    interface = (struct Eco_Interface*) Eco_Any_AsPointer(any);

    if (subindex == -1)
        any = Eco_Any_FromInteger(interface->entry_count);
    else if (subindex == -2)
        any = Eco_Any_FromPointer(interface->entries[index].key);
    else if (subindex == -3) {
        if (interface->entries[index].return_type == NULL)
            any = Eco_Any_FromPointer(Eco_Interface_GetDefaultInterface());
        else
            any = Eco_Any_FromPointer(interface->entries[index].return_type);
    }
    else if (subindex == -4)
        any = Eco_Any_FromInteger(interface->entries[index].arg_count);
    else if (subindex == -5)
        any = Eco_Any_FromInteger(interface->entries[index].has_varargs);
    else if (subindex < 0 || ((unsigned int) subindex) >= interface->entries[index].arg_count)
        any = Eco_Any_FromPointer(Eco_Interface_GetDefaultInterface());
    else {
        if (interface->entries[index].arg_types[subindex] == NULL)
            any = Eco_Any_FromPointer(Eco_Interface_GetDefaultInterface());
        else
            any = Eco_Any_FromPointer(interface->entries[index].arg_types[subindex]);
    }
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_InterfaceAddEntry(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_InterfaceEntry  entry;
    struct Eco_Interface*      interface;
    struct Eco_Array*          types;
    Eco_Any                    any;
    unsigned int               index;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 5, 5))
        return false;
    
    Eco_Fiber_Pop(fiber, &any);
    entry.has_varargs = Eco_Any_AsInteger(any) != 0;

    Eco_Fiber_Pop(fiber, &any);
    types = (struct Eco_Array*) Eco_Any_AsPointer(any);

    entry.arg_count = Eco_Array_Size(types);
    
    for (index = 0; index < entry.arg_count; index++)
    {
        entry.arg_types[index] = (struct Eco_Interface*) Eco_Any_AsPointer(*Eco_Array_At(types, index));
    }

    Eco_Fiber_Pop(fiber, &any);
    entry.key = (struct Eco_Key*) Eco_Any_AsPointer(any);

    Eco_Fiber_Pop(fiber, &any);
    entry.return_type = (struct Eco_Interface*) Eco_Any_AsPointer(any);

    Eco_Fiber_Pop(fiber, &any);
    interface = (struct Eco_Interface*) Eco_Any_AsPointer(any);

    interface = Eco_Interface_AddEntry(interface, &entry);

    // TODO, FIXME, XXX: Check if interface == NULL!

    any = Eco_Any_FromPointer(interface);
    Eco_Fiber_Push(fiber, &any);
    return true;
}
