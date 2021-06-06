#pragma once

enum Eco_Bytecode
{
    Eco_Bytecode_NOOP = 0,
    Eco_Bytecode_SELF,
    Eco_Bytecode_R2R,
    Eco_Bytecode_R2D,
    Eco_Bytecode_D2R,
    Eco_Bytecode_SEND,
    Eco_Bytecode_ASSIGN,
    Eco_Bytecode_RETURN
};

