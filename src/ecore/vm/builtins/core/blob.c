#include "blob.h"

#include <ecore/objects/misc/blob/blob.h>


bool Eco_VM_Builtin_BlobNew(struct Eco_Fiber* fiber, unsigned int args)
{
    /*
     * TODO: Type checks
     */
    Eco_Any  length;
    Eco_Any  array;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Fiber_Pop(fiber, &length);
    Eco_Any_AssignPointer(&array, (struct Eco_Object*) Eco_Blob_New(Eco_Any_AsInteger(&length)));
    Eco_Fiber_Push(fiber, &array);
    return true;
}

bool Eco_VM_Builtin_BlobSize(struct Eco_Fiber* fiber, unsigned int args)
{
    /*
     * TODO: Type checks
     */
    Eco_Any  array;
    Eco_Any  length;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Fiber_Pop(fiber, &array);
    Eco_Any_AssignInteger(&length, Eco_Blob_Size((struct Eco_Blob*) Eco_Any_AsPointer(&array)));
    Eco_Fiber_Push(fiber, &length);
    return true;
}
