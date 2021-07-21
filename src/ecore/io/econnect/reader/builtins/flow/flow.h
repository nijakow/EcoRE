#ifndef ECO_IO_ECONNECT_READER_BUILTINS_FLOW_FLOW_H
#define ECO_IO_ECONNECT_READER_BUILTINS_FLOW_FLOW_H

#include "../builtins_common_header.h"

bool Eco_EConnect_Builtin_Block(struct Eco_EConnect_Reader* reader,
                                struct Eco_EConnect_Result* result);
bool Eco_EConnect_Builtin_Unwind(struct Eco_EConnect_Reader* reader,
                                 struct Eco_EConnect_Result* result);

#endif
