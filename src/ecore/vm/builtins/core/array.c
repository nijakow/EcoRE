#include "array.h"

#include <ecore/objects/misc/array/array.h>


bool Eco_VM_Builtin_ArrayNew(struct Eco_Fiber* fiber, unsigned int args)
{
    /*
     * TODO: Type checks
     */
    Eco_Any  length;
    Eco_Any  array;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Fiber_Pop(fiber, &length);
    array = Eco_Any_FromPointer(Eco_Array_New(Eco_Any_AsInteger(length)));
    Eco_Fiber_Push(fiber, &array);
    return true;
}

bool Eco_VM_Builtin_ArraySize(struct Eco_Fiber* fiber, unsigned int args)
{
    /*
     * TODO: Type checks
     */
    Eco_Any  array;
    Eco_Any  length;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Fiber_Pop(fiber, &array);
    length = Eco_Any_FromInteger(Eco_Array_Size(Eco_Any_AsPointer(array)));
    Eco_Fiber_Push(fiber, &length);
    return true;
}

bool Eco_VM_Builtin_ArrayAt(struct Eco_Fiber* fiber, unsigned int args)
{
    /*
     * TODO: Type checks
     */
    Eco_Any  array;
    Eco_Any  index;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    Eco_Fiber_Pop(fiber, &index);
    Eco_Fiber_Pop(fiber, &array);
    Eco_Fiber_Push(fiber, Eco_Array_At(Eco_Any_AsPointer(array), Eco_Any_AsInteger(index)));
    return true;
}

bool Eco_VM_Builtin_ArrayAtPut(struct Eco_Fiber* fiber, unsigned int args)
{
    /*
     * TODO: Type checks
     */
    Eco_Any  index;
    Eco_Any  value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 3, 3))
        return false;
    Eco_Fiber_Pop(fiber, &value);
    Eco_Fiber_Pop(fiber, &index);
    Eco_Array_Put((Eco_Any_AsPointer(*Eco_Fiber_Peek(fiber))),
                   Eco_Any_AsInteger(index),
                   value);
    return true;
}

bool Eco_VM_Builtin_ArrayToString(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_String*  string;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;

    string = Eco_Array_ToString(Eco_Any_AsPointer(*Eco_Fiber_Peek(fiber)));

    *Eco_Fiber_Peek(fiber) = Eco_Any_FromPointer(string);
    
    return true;
}
