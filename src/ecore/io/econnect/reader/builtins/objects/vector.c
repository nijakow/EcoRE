#include "vector.h"

#include <ecore/io/econnect/reader/parser.h>

#include <ecore/objects/misc/vector/vector.h>


bool Eco_EConnect_Builtin_GetVector(struct Eco_EConnect_Reader* reader,
                                    struct Eco_EConnect_Result* result)
{
    unsigned int        id;
    unsigned int        index;
    unsigned int        elements;
    struct Eco_Vector*  vector;
    Eco_Any             value;

    id        = Eco_EConnect_ParseUInt(&reader->stream);

    elements  = Eco_EConnect_ParseUInt(&reader->stream);

    vector    = Eco_Vector_New(elements);

    if (vector != NULL) {
        Eco_EConnect_Instance_OptionallyBindObject(reader->instance, (struct Eco_Object*) vector, id);
        for (index = 0; index < elements; index++)
        {
            if (!Eco_EConnect_Reader_ReadAny(reader, result, &value))
                return false;
            Eco_Vector_Put(vector, index, &value);
        }
        Eco_EConnect_Result_Create_Object(result, (struct Eco_Object*) vector);
    } else {
        Eco_EConnect_Result_Create_Error(result, Eco_EConnect_ErrorType_INSTANTIATION_ERROR);
    }

    return true;
}
