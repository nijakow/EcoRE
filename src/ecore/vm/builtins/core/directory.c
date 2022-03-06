#include "directory.h"

#include <ecore/objects/misc/string/string.h>
#include <ecore/objects/io/directory.h>


bool Eco_VM_Builtin_Directory_Open(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any                any;
    struct Eco_String*     string;
    struct Eco_Directory*  directory;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Fiber_Pop(fiber, &any);
    string    = (struct Eco_String*) Eco_Any_AsPointer(&any);
    directory = Eco_Directory_New(string->bytes);
    Eco_Any_AssignPointer(&any, (struct Eco_Object*) directory);
    Eco_Fiber_Push(fiber, &any);
    return true;
}
