#ifndef ECO_IO_ECONNECT_READER_BUILTINS_ID_ID_H
#define ECO_IO_ECONNECT_READER_BUILTINS_ID_ID_H

#include "../builtins_common_header.h"


bool Eco_EConnect_Builtin_NewID(struct Eco_EConnect_Reader* reader,
                                struct Eco_EConnect_Result* result);
bool Eco_EConnect_Builtin_DelID(struct Eco_EConnect_Reader* reader,
                                struct Eco_EConnect_Result* result);
bool Eco_EConnect_Builtin_GetID(struct Eco_EConnect_Reader* reader,
                                struct Eco_EConnect_Result* result);

#endif
