#pragma once

#include "builtins_common_header.h"

bool Eco_EConnect_Builtin_Block(struct Eco_EConnect_Reader* reader,
                                struct Eco_EConnect_Result* result);
bool Eco_EConnect_Builtin_Unwind(struct Eco_EConnect_Reader* reader,
                                 struct Eco_EConnect_Result* result);
