#ifndef ECO_VM_CORE_BYTECODES_H
#define ECO_VM_CORE_BYTECODES_H

enum Eco_Bytecode
{
    Eco_Bytecode_NOOP = 0,
    Eco_Bytecode_SELF,
    Eco_Bytecode_C2R,
    Eco_Bytecode_R2R,
    Eco_Bytecode_R2L,
    Eco_Bytecode_L2R,
    Eco_Bytecode_A2R,
    Eco_Bytecode_SEND,
    Eco_Bytecode_ASSIGN,
    Eco_Bytecode_RETURN,
    Eco_Bytecode_MAKE_CLOSURE
};

#endif
