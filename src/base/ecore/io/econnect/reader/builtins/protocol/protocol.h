#ifndef ECO_IO_ECONNECT_READER_BUILTINS_PROTOCOL_PROTOCOL_H
#define ECO_IO_ECONNECT_READER_BUILTINS_PROTOCOL_PROTOCOL_H


#include <ecore/io/econnect/reader/builtins/builtins_common_header.h>


bool Eco_EConnect_Builtin_Null(struct Eco_EConnect_Reader* reader,
                               struct Eco_EConnect_Result* result);
bool Eco_EConnect_Builtin_Hello(struct Eco_EConnect_Reader* reader,
                                struct Eco_EConnect_Result* result);

#endif
