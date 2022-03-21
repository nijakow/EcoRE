#include "blob.h"

#include <ecore/objects/misc/blob/blob.h>
#include <ecore/objects/misc/string/string.h>


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

bool Eco_VM_Builtin_BlobAtPutInt8(struct Eco_Fiber* fiber, unsigned int args)
{
    /*
     * TODO: Type checks
     */
    Eco_Any  index;
    Eco_Any  value;
    i8       actual_value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 3, 3))
        return false;
    Eco_Fiber_Pop(fiber, &value);
    Eco_Fiber_Pop(fiber, &index);
    actual_value = Eco_Any_AsInteger(&value);
    Eco_Blob_AtPut(((struct Eco_Blob*) Eco_Any_AsPointer(Eco_Fiber_Peek(fiber))),
                   Eco_Any_AsInteger(&index),
                   &actual_value,
                   sizeof(actual_value));
    return true;
}

bool Eco_VM_Builtin_BlobAtPutInt16(struct Eco_Fiber* fiber, unsigned int args)
{
    /*
     * TODO: Type checks
     */
    Eco_Any  index;
    Eco_Any  value;
    i16      actual_value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 3, 3))
        return false;
    Eco_Fiber_Pop(fiber, &value);
    Eco_Fiber_Pop(fiber, &index);
    actual_value = Eco_Any_AsInteger(&value);
    Eco_Blob_AtPut(((struct Eco_Blob*) Eco_Any_AsPointer(Eco_Fiber_Peek(fiber))),
                   Eco_Any_AsInteger(&index),
                   &actual_value,
                   sizeof(actual_value));
    return true;
}

bool Eco_VM_Builtin_BlobAtPutInt32(struct Eco_Fiber* fiber, unsigned int args)
{
    /*
     * TODO: Type checks
     */
    Eco_Any  index;
    Eco_Any  value;
    i32      actual_value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 3, 3))
        return false;
    Eco_Fiber_Pop(fiber, &value);
    Eco_Fiber_Pop(fiber, &index);
    actual_value = Eco_Any_AsInteger(&value);
    Eco_Blob_AtPut(((struct Eco_Blob*) Eco_Any_AsPointer(Eco_Fiber_Peek(fiber))),
                   Eco_Any_AsInteger(&index),
                   &actual_value,
                   sizeof(actual_value));
    return true;
}

bool Eco_VM_Builtin_BlobReadFrom(struct Eco_Fiber* fiber, unsigned int args)
{
    int               fd;
    struct Eco_Blob*  blob;
    Eco_Any           any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    Eco_Fiber_Pop(fiber, &any);
    fd = Eco_Any_AsInteger(&any);
    Eco_Fiber_Pop(fiber, &any);
    blob = Eco_Any_AsPointer(&any);
    Eco_Any_AssignInteger(&any, Eco_Blob_ReadFromFD(blob, fd));
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_BlobWriteTo(struct Eco_Fiber* fiber, unsigned int args)
{
    int               fd;
    struct Eco_Blob*  blob;
    Eco_Any           any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    Eco_Fiber_Pop(fiber, &any);
    fd = Eco_Any_AsInteger(&any);
    Eco_Fiber_Pop(fiber, &any);
    blob = Eco_Any_AsPointer(&any);
    Eco_Any_AssignInteger(&any, Eco_Blob_WriteToFD(blob, fd));
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_BlobOpenFile(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any             any;
    struct Eco_String*  file_name;
    struct Eco_Blob*    blob;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Fiber_Pop(fiber, &any);
    file_name = (struct Eco_String*) Eco_Any_AsPointer(&any);
    blob      = Eco_Blob_NewFromFile(file_name->bytes); // TODO, FIXME, XXX! This is unsafe!
    Eco_Any_AssignPointer(&any, (struct Eco_Object*) blob);
    Eco_Fiber_Push(fiber, &any);
    return true;
}
