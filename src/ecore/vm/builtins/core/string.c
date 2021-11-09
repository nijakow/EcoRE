#include "string.h"

#include <ecore/objects/misc/string/string.h>


bool Eco_VM_Builtin_StringAtByte(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any  string;
    Eco_Any  index;
    Eco_Any  codepoint;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    Eco_Fiber_Pop(fiber, &index);
    Eco_Fiber_Pop(fiber, &string);
    // TODO: Type checks!
    Eco_Any_AssignCharacter(&codepoint, Eco_String_At((struct Eco_String*) Eco_Any_AsPointer(&string), Eco_Any_AsInteger(&index)));
    Eco_Fiber_Push(fiber, &codepoint);
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
    Eco_Any_AssignInteger(&length, Eco_String_ByteCount((struct Eco_String*) Eco_Any_AsPointer(&string)));
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
    Eco_Any_AssignInteger(&length, Eco_String_CharacterCount((struct Eco_String*) Eco_Any_AsPointer(&string)));
    Eco_Fiber_Push(fiber, &length);
    return true;
}
