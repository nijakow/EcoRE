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
    blob = Eco_Any_FromPointer(Eco_Blob_New(Eco_Any_AsInteger(length)));
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
    length = Eco_Any_FromInteger(Eco_Blob_Size(Eco_Any_AsPointer(blob)));
    Eco_Fiber_Push(fiber, &length);
    return true;
}

bool Eco_VM_Builtin_BlobAtInt8(struct Eco_Fiber* fiber, unsigned int args)
{
    /*
     * TODO: Type checks
     */
    Eco_Any  blob;
    Eco_Any  index;
    i8       v;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    Eco_Fiber_Pop(fiber, &index);
    Eco_Fiber_Pop(fiber, &blob);
    Eco_Blob_AtN(Eco_Any_AsPointer(blob), Eco_Any_AsInteger(index), &v, sizeof(v));
    index = Eco_Any_FromInteger(v);
    Eco_Fiber_Push(fiber, &index);
    return true;
}

bool Eco_VM_Builtin_BlobAtInt16(struct Eco_Fiber* fiber, unsigned int args)
{
    /*
     * TODO: Type checks
     */
    Eco_Any  blob;
    Eco_Any  index;
    i16      v;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    Eco_Fiber_Pop(fiber, &index);
    Eco_Fiber_Pop(fiber, &blob);
    Eco_Blob_AtN(Eco_Any_AsPointer(blob), Eco_Any_AsInteger(index), &v, sizeof(v));
    index = Eco_Any_FromInteger(v);
    Eco_Fiber_Push(fiber, &index);
    return true;
}

bool Eco_VM_Builtin_BlobAtInt32(struct Eco_Fiber* fiber, unsigned int args)
{
    /*
     * TODO: Type checks
     */
    Eco_Any  blob;
    Eco_Any  index;
    i32      v;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    Eco_Fiber_Pop(fiber, &index);
    Eco_Fiber_Pop(fiber, &blob);
    Eco_Blob_AtN(Eco_Any_AsPointer(blob), Eco_Any_AsInteger(index), &v, sizeof(v));
    index = Eco_Any_FromInteger(v);
    Eco_Fiber_Push(fiber, &index);
    return true;
}

bool Eco_VM_Builtin_BlobAtAny(struct Eco_Fiber* fiber, unsigned int args)
{
    /*
     * TODO: Type checks
     */
    Eco_Any  blob;
    Eco_Any  index;
    Eco_Any  value;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    Eco_Fiber_Pop(fiber, &index);
    Eco_Fiber_Pop(fiber, &blob);
    Eco_Blob_AtN(Eco_Any_AsPointer(blob), Eco_Any_AsInteger(index), &value, sizeof(value));
    Eco_Fiber_Push(fiber, &value);
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
    actual_value = Eco_Any_AsInteger(value);
    Eco_Blob_AtPut(Eco_Any_AsPointer(*Eco_Fiber_Peek(fiber)),
                   Eco_Any_AsInteger(index),
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
    actual_value = Eco_Any_AsInteger(value);
    Eco_Blob_AtPut(Eco_Any_AsPointer(*Eco_Fiber_Peek(fiber)),
                   Eco_Any_AsInteger(index),
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
    actual_value = Eco_Any_AsInteger(value);
    Eco_Blob_AtPut(Eco_Any_AsPointer(*Eco_Fiber_Peek(fiber)),
                   Eco_Any_AsInteger(index),
                   &actual_value,
                   sizeof(actual_value));
    return true;
}

bool Eco_VM_Builtin_BlobAtPutAny(struct Eco_Fiber* fiber, unsigned int args)
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
    Eco_Blob_AtPut(Eco_Any_AsPointer(*Eco_Fiber_Peek(fiber)),
                   Eco_Any_AsInteger(index),
                   &value,
                   sizeof(value));
    return true;
}

bool Eco_VM_Builtin_BlobReadFrom(struct Eco_Fiber* fiber, unsigned int args)
{
    int               fd;
    Eco_Integer       offset;
    Eco_Integer       bytes;
    struct Eco_Blob*  blob;
    Eco_Any           any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 4, 4))
        return false;
    Eco_Fiber_Pop(fiber, &any);
    bytes = (size_t) Eco_Any_AsInteger(any);
    Eco_Fiber_Pop(fiber, &any);
    offset = Eco_Any_AsInteger(any);
    Eco_Fiber_Pop(fiber, &any);
    fd = Eco_Any_AsInteger(any);
    Eco_Fiber_Pop(fiber, &any);
    blob = Eco_Any_AsPointer(any);
    any = Eco_Any_FromInteger(Eco_Blob_ReadFromFD(blob, fd, offset, bytes));
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_BlobWriteTo(struct Eco_Fiber* fiber, unsigned int args)
{
    int               fd;
    Eco_Integer       offset;
    Eco_Integer       bytes;
    struct Eco_Blob*  blob;
    Eco_Any           any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 4, 4))
        return false;
    Eco_Fiber_Pop(fiber, &any);
    bytes = Eco_Any_AsInteger(any);
    Eco_Fiber_Pop(fiber, &any);
    offset = Eco_Any_AsInteger(any);
    Eco_Fiber_Pop(fiber, &any);
    fd = Eco_Any_AsInteger(any);
    Eco_Fiber_Pop(fiber, &any);
    blob = Eco_Any_AsPointer(any);
    any = Eco_Any_FromInteger(Eco_Blob_WriteToFD(blob, fd, offset, bytes));
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
    file_name = Eco_Any_AsPointer(any);
    blob      = Eco_Blob_NewFromFile(file_name->bytes); // TODO, FIXME, XXX! This is unsafe!
    any = Eco_Any_FromPointer(blob);
    Eco_Fiber_Push(fiber, &any);
    return true;
}
