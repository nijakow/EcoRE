#include "blob.h"

#include <ecore/objects/misc/blob/blob.h>


bool Eco_VM_Builtin_BlobNew(struct Eco_Fiber* fiber, unsigned int args)
{
    /*
     * TODO: Type checks
     */
    Eco_Any  length;
    Eco_Any  blob;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Fiber_Pop(fiber, &length);
    Eco_Any_AssignPointer(&blob, (struct Eco_Object*) Eco_Blob_New(Eco_Any_AsInteger(&length)));
    Eco_Fiber_Push(fiber, &blob);
    return true;
}

bool Eco_VM_Builtin_BlobSize(struct Eco_Fiber* fiber, unsigned int args)
{
    /*
     * TODO: Type checks
     */
    Eco_Any  blob;
    Eco_Any  length;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Fiber_Pop(fiber, &blob);
    Eco_Any_AssignInteger(&length, Eco_Blob_Size((struct Eco_Blob*) Eco_Any_AsPointer(&blob)));
    Eco_Fiber_Push(fiber, &length);
    return true;
}

bool Eco_VM_Builtin_BlobAt(struct Eco_Fiber* fiber, unsigned int args)
{
    /*
     * TODO: Type checks
     */
    Eco_Any  blob;
    Eco_Any  index;
    char     v;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    Eco_Fiber_Pop(fiber, &index);
    Eco_Fiber_Pop(fiber, &blob);
    Eco_Blob_At((struct Eco_Blob*) Eco_Any_AsPointer(&blob), Eco_Any_AsInteger(&index), &v);
    Eco_Any_AssignInteger(&index, v);
    Eco_Fiber_Push(fiber, &index);
    return true;
}

bool Eco_VM_Builtin_BlobAtPutS8(struct Eco_Fiber* fiber, unsigned int args)
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
    Eco_Blob_AtPutS8(((struct Eco_Blob*) Eco_Any_AsPointer(Eco_Fiber_Peek(fiber))),
                     Eco_Any_AsInteger(&index),
                     Eco_Any_AsInteger(&value));
    return true;
}
