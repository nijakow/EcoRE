#ifndef ECO_VM_CORE_BYTECODES_H
#define ECO_VM_CORE_BYTECODES_H

#include <ecore/base/defs.h>

enum Eco_Bytecode
{
    Eco_Bytecode_NOOP = 0,
    Eco_Bytecode_CONST,
    Eco_Bytecode_PUSH,
    Eco_Bytecode_POP,
    Eco_Bytecode_LOAD_LOCAL,
    Eco_Bytecode_STORE_LOCAL,
    Eco_Bytecode_LOAD_LEXICAL,
    Eco_Bytecode_STORE_LEXICAL,
    Eco_Bytecode_BUILTIN,
    Eco_Bytecode_BUILTINV,
    Eco_Bytecode_SEND,
    Eco_Bytecode_SENDV,
    Eco_Bytecode_RESEND,
    Eco_Bytecode_RESENDV,
    Eco_Bytecode_ASSIGN,
    Eco_Bytecode_RETURN,
    Eco_Bytecode_MAKE_CLOSURE,

    /*
     * Extended bytecode set starts at 0x40, giving us 64 basic opcodes to work with.
     */
    Eco_Bytecode_JUMP = 0x40,
};

static inline bool Eco_Bytecode_IsExtended(enum Eco_Bytecode bytecode)
{
    return bytecode >= 0x40;
}

#endif
