#include "port.h"

#include <ecore/objects/io/port.h>


bool Eco_VM_Builtin_NewPort(struct Eco_Fiber* fiber, unsigned int args)
{
    unsigned int  fd;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    fd = Eco_Any_AsInteger(Eco_Fiber_Peek(fiber));
    Eco_Any_AssignPointer(Eco_Fiber_Peek(fiber), (struct Eco_Object*) Eco_Port_New(fd));
    return true;
}

bool Eco_VM_Builtin_PortReadChar(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Codepoint  codepoint;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Port_ReadChar((struct Eco_Port*) Eco_Any_AsPointer(Eco_Fiber_Peek(fiber)), &codepoint);
    Eco_Any_AssignCharacter(Eco_Fiber_Peek(fiber), codepoint);
    return true;
}

bool Eco_VM_Builtin_PortWriteChar(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  character;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Fiber_Pop(fiber, &character);
    Eco_Port_WriteChar((struct Eco_Port*) Eco_Any_AsPointer(Eco_Fiber_Peek(fiber)), Eco_Any_AsCharacter(&character));
    return true;
}
