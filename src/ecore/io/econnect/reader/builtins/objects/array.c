#include "array.h"

#include <ecore/io/econnect/reader/parser.h>

#include <ecore/objects/misc/array/array.h>

bool Eco_EConnect_Builtin_GetArray(struct Eco_EConnect_Reader* reader,
                                   struct Eco_EConnect_Result* result)
{
    unsigned int       id;
    unsigned int       index;
    unsigned int       elements;
    struct Eco_Array*  array;
    Eco_Any            value;

    id        = Eco_EConnect_ParseUInt(&reader->stream);
    elements  = Eco_EConnect_ParseUInt(&reader->stream);

    array     = Eco_Array_New(elements);

    if (array != NULL) {
        Eco_EConnect_Instance_OptionallyBindObject(reader->instance, (struct Eco_Object*) array, id);
        for (index = 0; index < elements; index++)
        {
            if (!Eco_EConnect_Reader_ReadAny(reader, result, &value))
                return false;
            Eco_Array_Put(array, index, value);
        }
        Eco_EConnect_Result_Create_Object(result, (struct Eco_Object*) array);
    } else {
        Eco_EConnect_Result_Create_Error(result, Eco_EConnect_ErrorType_INSTANTIATION_ERROR);
    }

    return true;
}
