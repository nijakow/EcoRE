#include "vector.h"

#include <ecore/objects/misc/vector/vector.h>


bool Eco_VM_Builtin_VectorSize(struct Eco_Fiber* fiber, unsigned int args)
{
    /*
     * TODO: Type checks
     */
    Eco_Any  vector;
    Eco_Any  length;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Fiber_Pop(fiber, &vector);
    Eco_Any_AssignInteger(&length, Eco_Vector_Size((struct Eco_Vector*) Eco_Any_AsPointer(&vector)));
    Eco_Fiber_Push(fiber, &length);
    return true;
}

bool Eco_VM_Builtin_VectorAt(struct Eco_Fiber* fiber, unsigned int args)
{
    /*
     * TODO: Type checks
     */
    Eco_Any  vector;
    Eco_Any  index;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    Eco_Fiber_Pop(fiber, &index);
    Eco_Fiber_Pop(fiber, &vector);
    Eco_Fiber_Push(fiber, Eco_Vector_At((struct Eco_Vector*) Eco_Any_AsPointer(&vector), Eco_Any_AsInteger(&index)));
    return true;
}

bool Eco_VM_Builtin_VectorAtPut(struct Eco_Fiber* fiber, unsigned int args)
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
    Eco_Vector_Put(((struct Eco_Vector*) Eco_Any_AsPointer(Eco_Fiber_Peek(fiber))),
                   Eco_Any_AsInteger(&index),
                   &value);
    return true;
}

bool Eco_VM_Builtin_VectorInsert(struct Eco_Fiber* fiber, unsigned int args)
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
    Eco_Vector_Insert(((struct Eco_Vector*) Eco_Any_AsPointer(Eco_Fiber_Peek(fiber))),
                      Eco_Any_AsInteger(&index),
                      &value);
    return true;
}

bool Eco_VM_Builtin_VectorRemove(struct Eco_Fiber* fiber, unsigned int args)
{
    /*
     * TODO: Type checks
     */
    Eco_Any  index;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    Eco_Fiber_Pop(fiber, &index);
    Eco_Vector_Remove(((struct Eco_Vector*) Eco_Any_AsPointer(Eco_Fiber_Peek(fiber))),
                      Eco_Any_AsInteger(&index));
    return true;
}

bool Eco_VM_Builtin_VectorToString(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_String*  string;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;

    string = Eco_Vector_ToString((struct Eco_Vector*) Eco_Any_AsPointer(Eco_Fiber_Peek(fiber)));

    Eco_Any_AssignPointer(Eco_Fiber_Peek(fiber), (struct Eco_Object*) string);
    
    return true;
}
