#pragma once

#include "builtins_common_header.h"


bool Eco_EConnect_Builtin_NewID(struct Eco_EConnect_Reader* reader,
                                struct Eco_EConnect_Result* result);
bool Eco_EConnect_Builtin_DelID(struct Eco_EConnect_Reader* reader,
                                struct Eco_EConnect_Result* result);
bool Eco_EConnect_Builtin_GetID(struct Eco_EConnect_Reader* reader,
                                struct Eco_EConnect_Result* result);
