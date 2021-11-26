#include "port.h"

#include <ecore/objects/io/port.h>
#include <ecore/vm/fiber/sched.h>
#include <ecore/vm/vm.h>


bool Eco_VM_Builtin_NewPort(struct Eco_Fiber* fiber, unsigned int args)
{
    unsigned int  fd;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    fd = Eco_Any_AsInteger(Eco_Fiber_Peek(fiber));
    Eco_Any_AssignPointer(Eco_Fiber_Peek(fiber), (struct Eco_Object*) Eco_Port_New(Eco_VM_GetScheduler(fiber->vm), fd));
    return true;
}

bool Eco_VM_Builtin_PortReadByte(struct Eco_Fiber* fiber, unsigned int args)
{
    char              byte;
    Eco_Any           any;
    struct Eco_Port*  port;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Fiber_Pop(fiber, &any);
    port = (struct Eco_Port*) Eco_Any_AsPointer(&any);
    if (Eco_Port_ReadByte(port, &byte)) {
        Eco_Any_AssignInteger(&any, ((unsigned char) byte));
    } else {
        Eco_Any_AssignAny(&any, &fiber->vm->constants.cfalse);
    }
    Eco_Fiber_Push(fiber, &any);
    return true;
}

bool Eco_VM_Builtin_PortFlushOutput(struct Eco_Fiber* fiber, unsigned int args)
{
    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Port_FlushOutput((struct Eco_Port*) Eco_Any_AsPointer(Eco_Fiber_Peek(fiber)));
    return true;
}

bool Eco_VM_Builtin_PortWriteChar(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  character;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    Eco_Fiber_Pop(fiber, &character);
    Eco_Port_WriteChar((struct Eco_Port*) Eco_Any_AsPointer(Eco_Fiber_Peek(fiber)), Eco_Any_AsCharacter(&character));
    return true;
}

bool Eco_VM_Builtin_PortWriteByte(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  byte;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    Eco_Fiber_Pop(fiber, &byte);
    Eco_Port_WriteByte((struct Eco_Port*) Eco_Any_AsPointer(Eco_Fiber_Peek(fiber)), Eco_Any_AsInteger(&byte));
    return true;
}

bool Eco_VM_Builtin_PortNotifyMe(struct Eco_Fiber* fiber, unsigned int args)
{
    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Port_QueueFiber((struct Eco_Port*) Eco_Any_AsPointer(Eco_Fiber_Peek(fiber)), fiber); // TODO: Handle failure
    return true;
}
