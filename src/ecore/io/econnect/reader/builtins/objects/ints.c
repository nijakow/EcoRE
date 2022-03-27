#include "ints.h"

#include <ecore/io/econnect/reader/parser.h>
#include <ecore/io/logging/log.h>


bool Eco_EConnect_Builtin_GetUInt(struct Eco_EConnect_Reader* reader,
                                  struct Eco_EConnect_Result* result)
{
    unsigned int  value;

    value = Eco_EConnect_ParseUInt(&reader->stream);
    Eco_EConnect_Result_Create_Integer(result, value);

    return true;
}

bool Eco_EConnect_Builtin_GetInt(struct Eco_EConnect_Reader* reader,
                                 struct Eco_EConnect_Result* result)
{
    int  value;

    value = Eco_EConnect_ParseInt(&reader->stream);
    Eco_EConnect_Result_Create_Integer(result, value);

    return true;
}

bool Eco_EConnect_Builtin_GetCharacter(struct Eco_EConnect_Reader* reader,
                                       struct Eco_EConnect_Result* result)
{
    Eco_Codepoint  value;

    value = Eco_EConnect_ParseUInt(&reader->stream);
    Eco_EConnect_Result_Create_Character(result, value);

    return true;
}
