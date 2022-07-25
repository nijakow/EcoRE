#include "reflection.h"

#include <ecore/base/extra.h>
#include <ecore/objects/base/type/type.h>
#include <ecore/objects/base/type/slot.h>
#include <ecore/objects/misc/array/array.h>
#include <ecore/objects/vm/interface/interface.h>
#include <ecore/vm/vm.h>

bool Eco_VM_Builtin_GetSlotValue(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_Key*       name;
    struct Eco_Molecule*  molecule;
    struct Eco_Type*      type;
    Eco_Any               any;
    unsigned int          index;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    any  = Eco_Fiber_Pop(fiber);
    name = Eco_Any_AsPointer(any);
    any  = Eco_Fiber_Pop(fiber);
    type = Eco_Any_GetType(any);
    if (type->proxy != NULL)
        molecule = (struct Eco_Molecule*) type->proxy;
    else
        molecule = Eco_Any_AsPointer(any);
    for (index = 0; index < type->slot_count; index++)
    {
        if (type->slots[index].info.key == name)
        {
            if (type->slots[index].type == Eco_TypeSlotType_CODE)
                any = Eco_Any_FromPointer(type->slots[index].body.code.code);
            else
                Eco_TypeSlot_GetValue(&type->slots[index], molecule, &any);
            break;
        }
    }
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_GetInterface(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_Interface* interface;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    interface = Eco_Type_GetInterface(Eco_Any_AsPointer(*Eco_Fiber_Peek(fiber)), true);
    *Eco_Fiber_Peek(fiber) = Eco_Any_FromPointer(interface);
    return true;
}

bool Eco_VM_Builtin_GetPublicInterface(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_Interface* interface;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    interface = Eco_Type_GetInterface(Eco_Any_AsPointer(*Eco_Fiber_Peek(fiber)), false);
    *Eco_Fiber_Peek(fiber) = Eco_Any_FromPointer(interface);
    return true;
}

bool Eco_VM_Builtin_GetType(struct Eco_Fiber* fiber, unsigned int args)
{
    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    *Eco_Fiber_Peek(fiber) = Eco_Any_FromPointer(Eco_Any_GetType(*Eco_Fiber_Peek(fiber)));
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
    any   = Eco_Fiber_Pop(fiber);
    type  = (struct Eco_Type*) Eco_Any_AsPointer(any);
    array = Eco_Array_New(type->slot_count);
    // TODO, FIXME, XXX: Handle array == NULL!
    for (index = 0; index < type->slot_count; index++)
    {
        any = Eco_Any_FromPointer(type->slots[index].info.key);
        Eco_Array_Put(array, index, any);
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
    
    any      = Eco_Fiber_Pop(fiber);
    subindex = Eco_Any_AsInteger(any);
    any      = Eco_Fiber_Pop(fiber);
    index    = Eco_Any_AsInteger(any);
    any      = Eco_Fiber_Pop(fiber);
    type     = Eco_Any_AsPointer(any);

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

bool Eco_VM_Builtin_GetTypeSupers(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_Type*   type;
    struct Eco_Array*  elements;
    Eco_Any            any;
    unsigned int       index;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    any      = Eco_Fiber_Pop(fiber);
    type     = Eco_Any_AsPointer(any);
    elements = Eco_Array_New(type->inherited_types.fill);
    for (index = 0; index < type->inherited_types.fill; index++)
    {
        any = Eco_Any_FromPointer(type->inherited_types.types[index]);
        Eco_Array_Put(elements, index, any);
    }
    any = Eco_Any_FromPointer(elements);
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_GetTypeSubs(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_Type*   type;
    struct Eco_Array*  elements;
    Eco_Any            any;
    unsigned int       index;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    any      = Eco_Fiber_Pop(fiber);
    type     = Eco_Any_AsPointer(any);
    elements = Eco_Array_New(type->inheriting_types.fill);
    for (index = 0; index < type->inheriting_types.fill; index++)
    {
        any = Eco_Any_FromPointer(type->inheriting_types.types[index]);
        Eco_Array_Put(elements, index, any);
    }
    any = Eco_Any_FromPointer(elements);
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_GetTypeProxy(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_Type*  type;
    Eco_Any           any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    any  = Eco_Fiber_Pop(fiber);
    type = Eco_Any_AsPointer(any);
    if (type->proxy != NULL)    // If NULL, any will point to the type itself
        any = Eco_Any_FromPointer(type->proxy);
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_InterfaceGetParents(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_Interface*  interface;
    struct Eco_Array*      parents;
    unsigned int           index;
    Eco_Any                any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    
    any       = Eco_Fiber_Pop(fiber);
    interface = Eco_Any_AsPointer(any);

    parents = Eco_Array_New(Eco_Interface_GetParentCount(interface));
    for (index = 0; index < Eco_Interface_GetParentCount(interface); index++)
        Eco_Array_Put(parents, index, Eco_Any_FromPointer(interface->parents[index]));
    
    any = Eco_Any_FromPointer(parents);
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
    
    any       = Eco_Fiber_Pop(fiber);
    subindex  = Eco_Any_AsInteger(any);
    any       = Eco_Fiber_Pop(fiber);
    index     = Eco_Any_AsInteger(any);
    any       = Eco_Fiber_Pop(fiber);
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

bool Eco_VM_Builtin_InterfaceAddParent(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_Interface*  old_interface;
    struct Eco_Interface*  new_interface;
    struct Eco_Interface*  parent;
    Eco_Any                any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;

    any           = Eco_Fiber_Pop(fiber);
    parent        = Eco_Any_AsPointer(any);
    any           = Eco_Fiber_Pop(fiber);
    old_interface = Eco_Any_AsPointer(any);

    new_interface = Eco_Interface_AddParent(old_interface, parent);

    // TODO, FIXME, XXX: Check if new_interface == NULL!

    any = Eco_Any_FromPointer(new_interface);
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
    
    any               = Eco_Fiber_Pop(fiber);
    entry.has_varargs = Eco_Any_AsInteger(any) != 0;

    any   = Eco_Fiber_Pop(fiber);
    types = (struct Eco_Array*) Eco_Any_AsPointer(any);

    entry.arg_count = Eco_Array_Size(types);
    
    for (index = 0; index < entry.arg_count; index++)
    {
        entry.arg_types[index] = (struct Eco_Interface*) Eco_Any_AsPointer(*Eco_Array_At(types, index));
    }

    any       = Eco_Fiber_Pop(fiber);
    entry.key = (struct Eco_Key*) Eco_Any_AsPointer(any);

    any               = Eco_Fiber_Pop(fiber);
    entry.return_type = (struct Eco_Interface*) Eco_Any_AsPointer(any);

    any       = Eco_Fiber_Pop(fiber);
    interface = (struct Eco_Interface*) Eco_Any_AsPointer(any);

    interface = Eco_Interface_AddEntry(interface, &entry);

    // TODO, FIXME, XXX: Check if interface == NULL!

    any = Eco_Any_FromPointer(interface);
    Eco_Fiber_Push(fiber, &any);
    return true;
}


bool Eco_VM_Builtin_InterfaceImplementsMessage(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_Interface*  interface;
    struct Eco_Key*        message;
    Eco_Any                any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    
    any     = Eco_Fiber_Pop(fiber);
    message = Eco_Any_AsPointer(any);

    any       = Eco_Fiber_Pop(fiber);
    interface = Eco_Any_AsPointer(any);

    if (Eco_Interface_ImplementsMessage(interface, message)) {
        any = fiber->vm->constants.ctrue;
    } else {
        any = fiber->vm->constants.cfalse;
    }

    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_InterfaceImplementsInterface(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_Interface*  interface;
    struct Eco_Interface*  subinterface;
    Eco_Any                any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    
    any          = Eco_Fiber_Pop(fiber);
    subinterface = Eco_Any_AsPointer(any);

    any       = Eco_Fiber_Pop(fiber);
    interface = Eco_Any_AsPointer(any);

    if (Eco_Interface_ImplementsInterface(interface, subinterface)) {
        any = fiber->vm->constants.ctrue;
    } else {
        any = fiber->vm->constants.cfalse;
    }

    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_InterfaceGetAllInterfaces(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 0, 0))
        return false;
    any = Eco_Any_FromPointer(Eco_Interface_GetAllInterfacesAsArray());
    Eco_Fiber_Push(fiber, &any);
    return true;
}
