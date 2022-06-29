#ifndef ECO_VM_CORE_BYTECODES_H
#define ECO_VM_CORE_BYTECODES_H

enum Eco_Bytecode
{
    Eco_Bytecode_NOOP = 0,
    Eco_Bytecode_CONST,
    Eco_Bytecode_PUSHC,
    Eco_Bytecode_PUSH,
    Eco_Bytecode_POP,
    Eco_Bytecode_DROP,
    Eco_Bytecode_DUP,
    Eco_Bytecode_R2R,
    Eco_Bytecode_R2L,
    Eco_Bytecode_L2R,
    Eco_Bytecode_A2R,
    Eco_Bytecode_R2A,
    Eco_Bytecode_BUILTIN,
    Eco_Bytecode_BUILTINV,
    Eco_Bytecode_SEND,
    Eco_Bytecode_SENDV,
    Eco_Bytecode_ASSIGN,
    Eco_Bytecode_AS,
    Eco_Bytecode_RETURN,
    Eco_Bytecode_MAKE_CLOSURE,
};

#endif
