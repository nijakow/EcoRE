#include "uint.h"

#include "../../parser.h"
#include "../../../../logging/log.h"


bool Eco_EConnect_Builtin_GetUInt(struct Eco_EConnect_Reader* reader,
                                  struct Eco_EConnect_Result* result)
{
    unsigned int  value;

    value = Eco_EConnect_ParseUInt(&reader->stream);
    Eco_EConnect_Result_Create_Integer(result, value);

    return true;
}
