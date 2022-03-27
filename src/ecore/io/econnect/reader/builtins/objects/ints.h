#ifndef ECO_IO_ECONNECT_READER_BUILTINS_OBJECTS_UINT_H
#define ECO_IO_ECONNECT_READER_BUILTINS_OBJECTS_UINT_H

#include <ecore/io/econnect/reader/builtins/builtins_common_header.h>

bool Eco_EConnect_Builtin_GetUInt(struct Eco_EConnect_Reader* reader,
                                  struct Eco_EConnect_Result* result);
bool Eco_EConnect_Builtin_GetInt(struct Eco_EConnect_Reader* reader,
                                 struct Eco_EConnect_Result* result);
bool Eco_EConnect_Builtin_GetCharacter(struct Eco_EConnect_Reader* reader,
                                       struct Eco_EConnect_Result* result);

#endif
