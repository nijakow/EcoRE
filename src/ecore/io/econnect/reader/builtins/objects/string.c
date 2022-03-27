#include "string.h"

#include <ecore/io/econnect/reader/parser.h>

#include <ecore/objects/misc/string/string.h>


bool Eco_EConnect_Builtin_GetString(struct Eco_EConnect_Reader* reader,
                                    struct Eco_EConnect_Result* result)
{
    unsigned int        id;
    unsigned int        bytes;
    struct Eco_String*  string;

    id     = Eco_EConnect_ParseUInt(&reader->stream);

    bytes  = Eco_EConnect_ParseUInt(&reader->stream);

    char                buffer[bytes + 1];
    Eco_EConnect_ParseString(&reader->stream, buffer, bytes);

    string = Eco_String_New(buffer);

    if (string != NULL) {
        Eco_EConnect_Instance_OptionallyBindObject(reader->instance, (struct Eco_Object*) string, id);
        Eco_EConnect_Result_Create_Object(result, (struct Eco_Object*) string);
    } else {
        Eco_EConnect_Result_Create_Error(result, Eco_EConnect_ErrorType_INSTANTIATION_ERROR);
    }

    return true;
}
