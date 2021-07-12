#include "flow.h"

#include "../parser.h"


bool Eco_EConnect_Builtin_Block(struct Eco_EConnect_Reader* reader,
                                struct Eco_EConnect_Result* result)
{
    unsigned int  elements;
    unsigned int  i;

    elements = Eco_EConnect_ParseUInt(&reader->stream);

    if (elements == 0) {
        Eco_EConnect_Result_Create_None(result);
        return true;
    }

    for (i = 0; i < elements - 1; i++)
    {
        if (Eco_EConnect_Reader_Read(reader, result)) {
            Eco_EConnect_Result_Destroy(result);
        } else {
            return false;
        }
    }

    return Eco_EConnect_Reader_Read(reader, result);
}

bool Eco_EConnect_Builtin_Unwind(struct Eco_EConnect_Reader* reader,
                                 struct Eco_EConnect_Result* result)
{
    struct Eco_EConnect_Result  throwaway_result;

    if (!Eco_EConnect_Reader_Read(reader, result)) {
        return false;
    }
    
    if (Eco_EConnect_Reader_Read(reader, &throwaway_result)) {
        Eco_EConnect_Result_Destroy(&throwaway_result);
        return true;
    } else {
        Eco_EConnect_Result_Destroy(result);
        Eco_EConnect_Result_Copy(result, &throwaway_result);
        return false;
    }
}
