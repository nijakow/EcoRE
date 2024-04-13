#include "reflection.h"

#include <ecore/base/extra.h>
#include <ecore/objects/base/type/type.h>
#include <ecore/objects/base/type/slot.h>
#include <ecore/objects/base/molecule/molecule.h>
#include <ecore/objects/misc/array/array.h>
#include <ecore/objects/vm/code/code.h>
#include <ecore/vm/vm.h>


bool Eco_VM_Builtin_IsMolecule(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    any = Eco_Fiber_Pop(fiber);
    if (Eco_Any_IsPointer(any) && Eco_Molecule_IsMolecule(Eco_Any_AsPointer(any)))
        Eco_Fiber_Push(fiber, Eco_Any_FromInteger(1));
    else
        Eco_Fiber_Push(fiber, Eco_Any_FromInteger(0));
    return true;
}

bool Eco_VM_Builtin_IsCode(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    any = Eco_Fiber_Pop(fiber);
    if (Eco_Any_IsPointer(any) && Eco_Code_IsCode(Eco_Any_AsPointer(any)))
        Eco_Fiber_Push(fiber, Eco_Any_FromInteger(1));
    else
        Eco_Fiber_Push(fiber, Eco_Any_FromInteger(0));
    return true;
}

bool Eco_VM_Builtin_GetProxy(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any           any;
    struct Eco_Type*  type;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    any  = Eco_Fiber_Pop(fiber);
    type = Eco_Any_GetType(any);

    if (type->proxy == NULL) {
        Eco_Fiber_Push(fiber, any);
    } else {
        Eco_Fiber_Push(fiber, Eco_Any_FromPointer(type->proxy));
    }

    return true;
}

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
    Eco_Fiber_Push(fiber, any);
    return true;
}

bool Eco_VM_Builtin_GetSlotFlags(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_Key*       name;
    struct Eco_Type*      type;
    Eco_Any               any;
    unsigned int          index;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    any  = Eco_Fiber_Pop(fiber);
    name = Eco_Any_AsPointer(any);
    any  = Eco_Fiber_Pop(fiber);
    type = Eco_Any_GetType(any);
    any  = Eco_Any_FromInteger(0);
    for (index = 0; index < type->slot_count; index++)
    {
        if (type->slots[index].info.key == name)
        {
            any = Eco_Any_FromInteger(Eco_SlotFlags_AsInteger(&type->slots[index].info.flags));
            break;
        }
    }
    Eco_Fiber_Push(fiber, any);
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
    Eco_Fiber_Push(fiber, any);
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
        Eco_Log_Error("Eco_VM_Builtin_GetTypeSlotInfo(): subindex == -3 used to be 'interface', IS NOW DEPRECATED!\n");
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
    Eco_Fiber_Push(fiber, any);
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
    Eco_Fiber_Push(fiber, any);
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
    Eco_Fiber_Push(fiber, any);
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
    Eco_Fiber_Push(fiber, any);
    return true;
}
