#include "string.h"

#include <ecore/objects/misc/string/string.h>


bool Eco_VM_Builtin_StringByteAt(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  string;
    Eco_Any  index;
    Eco_Any  codepoint;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    Eco_Fiber_Pop(fiber, &index);
    Eco_Fiber_Pop(fiber, &string);
    // TODO: Type checks!
    codepoint = Eco_Any_FromInteger(Eco_String_ByteAt(Eco_Any_AsPointer(string), Eco_Any_AsInteger(index)));
    Eco_Fiber_Push(fiber, &codepoint);
    return true;
}

bool Eco_VM_Builtin_StringCodepointAtByte(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  string;
    Eco_Any  index;
    Eco_Any  codepoint;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    Eco_Fiber_Pop(fiber, &index);
    Eco_Fiber_Pop(fiber, &string);
    // TODO: Type checks!
    codepoint = Eco_Any_FromCharacter(Eco_String_At(Eco_Any_AsPointer(string), Eco_Any_AsInteger(index)));
    Eco_Fiber_Push(fiber, &codepoint);
    return true;
}

bool Eco_VM_Builtin_StringStepAtByte(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  string;
    Eco_Any  index;
    Eco_Any  result;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    Eco_Fiber_Pop(fiber, &index);
    Eco_Fiber_Pop(fiber, &string);
    // TODO: Type checks!
    result = Eco_Any_FromInteger(Eco_String_StepAt(Eco_Any_AsPointer(string), Eco_Any_AsInteger(index)));
    Eco_Fiber_Push(fiber, &result);
    return true;
}

bool Eco_VM_Builtin_StringByteLength(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  string;
    Eco_Any  length;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Fiber_Pop(fiber, &string);
    // TODO: Type checks!
    length = Eco_Any_FromInteger(Eco_String_ByteCount(Eco_Any_AsPointer(string)));
    Eco_Fiber_Push(fiber, &length);
    return true;
}

bool Eco_VM_Builtin_StringCharLength(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  string;
    Eco_Any  length;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    Eco_Fiber_Pop(fiber, &string);
    // TODO: Type checks!
    length = Eco_Any_FromInteger(Eco_String_CharacterCount(Eco_Any_AsPointer(string)));
    Eco_Fiber_Push(fiber, &length);
    return true;
}

bool Eco_VM_Builtin_StringAsKey(struct Eco_Fiber* fiber, unsigned int args)
{
    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 1, 1))
        return false;
    *Eco_Fiber_Peek(fiber) = Eco_Any_FromPointer(Eco_String_AsKey(Eco_Any_AsPointer(*Eco_Fiber_Peek(fiber))));
    return true;
}
