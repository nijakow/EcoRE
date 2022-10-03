#ifndef ECO_VM_CORE_BYTECODES_H
#define ECO_VM_CORE_BYTECODES_H

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
};

#endif
