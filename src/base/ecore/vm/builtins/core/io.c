#include <fcntl.h>

#include "io.h"

#include <ecore/objects/misc/array/array.h>
#include <ecore/objects/misc/string/string.h>
#include <ecore/vm/fiber/stackops.h>
#include <ecore/vm/vm.h>
#include <ecore/io/logging/log.h>
#include <ecore/util/libc.h>


bool Eco_VM_Builtin_Print(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, ECO_VM_BUILTIN_INFINITE_ARGS))
        return false;
    
    any = Eco_Any_Default();

    while (args --> 0)
    {
        any = Eco_Fiber_Pop(fiber);
        if (Eco_Any_IsInteger(any)) {
            Eco_Log_Info("Integer: %d\n", Eco_Any_AsInteger(any));
        } else if (Eco_Any_IsFloating(any)) {
            Eco_Log_Info("Floating: %f\n", Eco_Any_AsFloating(any));
        } else if (Eco_Any_IsPointer(any)) {
            Eco_Log_Info("Pointer: %p\n", Eco_Any_AsPointer(any));
        } else if (Eco_Any_IsCharacter(any)) {
            Eco_Log_Info("Character: %-10d #\\%c\n", Eco_Any_AsCharacter(any), Eco_Any_AsCharacter(any));
        } else {
            Eco_Log_Info("???\n");
        }
    }

    Eco_Fiber_Push(fiber, any);

    return true;
}

bool Eco_VM_Builtin_OpenFile(struct Eco_Fiber* fiber, unsigned int args)
{
    int                 mask;
    int                 fd;
    Eco_Any             any;
    Eco_Integer         bits;
    struct Eco_String*  path;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    any  = Eco_Fiber_Pop(fiber);
    bits = Eco_Any_AsInteger(any);
    any  = Eco_Fiber_Pop(fiber);
    path = Eco_Any_AsPointer(any);

    char buffer[Eco_String_ByteCount(path) + 1];
    if (Eco_String_PutIntoByteArray(path, buffer, sizeof(buffer))) {
        mask = 0;
        if (bits & 0x01) mask |= O_RDONLY;
        if (bits & 0x02) mask |= O_WRONLY;
        if (bits & 0x04) mask |= O_CREAT;
        if (bits & 0x08) mask |= O_TRUNC;
        fd = open(buffer, mask, 0744);
    } else {
        fd = -1;
    }
    any = Eco_Any_FromInteger(fd);
    Eco_Fiber_Push(fiber, any);
    return true;
}

bool Eco_VM_Builtin_FileExists(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_String*  path;
    Eco_Any             any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    any  = Eco_Fiber_Pop(fiber);
    path = Eco_Any_AsPointer(any);
    if (Eco_LibC_FileExists(path->bytes))
        any = fiber->vm->constants.ctrue;
    else
        any = fiber->vm->constants.cfalse;
    Eco_Fiber_Push(fiber, any);
    return true;
}

bool Eco_VM_Builtin_FileIsDirectory(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_String*  path;
    Eco_Any             any;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    any  = Eco_Fiber_Pop(fiber);
    path = Eco_Any_AsPointer(any);
    if (Eco_LibC_FileIsDirectory(path->bytes))
        any = fiber->vm->constants.ctrue;
    else
        any = fiber->vm->constants.cfalse;
    Eco_Fiber_Push(fiber, any);
    return true;
}

bool Eco_VM_Builtin_ListFiles(struct Eco_Fiber* fiber, unsigned int args)
{
    struct Eco_String*  path;
    struct Eco_String*  element;
    struct Eco_Array*   elements;
    unsigned int        index;
    unsigned int        count;
    Eco_Any             any;
    char*               entries[256];

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    any  = Eco_Fiber_Pop(fiber);
    path = Eco_Any_AsPointer(any);
    if (!Eco_LibC_ListFiles(path->bytes, entries, 256))
        entries[0] = NULL;
    count = 0;
    while (entries[count] != NULL)
        count++;
    elements = Eco_Array_New(count);
    for (index = 0; index < count; index++)
    {
        element = Eco_String_New(entries[index]);
        Eco_Array_Put(elements, index, Eco_Any_FromPointer(element));
        Eco_LibC_Free(entries[index]);
    }
    any = Eco_Any_FromPointer(elements);
    Eco_Fiber_Push(fiber, any);
    return true;
}
