#include "interface.h"

#include <ecore/io/econnect/reader/parser.h>
#include <ecore/objects/vm/interface/interface.h>
#include <ecore/vm/memory/memory.h>


bool Eco_EConnect_Builtin_GetInterface(struct Eco_EConnect_Reader* reader,
                                       struct Eco_EConnect_Result* result)
{
    unsigned int               id;
    unsigned int               entry_count;
    unsigned int               index;
    unsigned int               arg;
    struct Eco_Interface*      the_interface;
    struct Eco_InterfaceEntry  entry;

    id            = Eco_EConnect_ParseUInt(&reader->stream);
    entry_count   = Eco_EConnect_ParseUInt(&reader->stream);
    the_interface = Eco_Interface_New(entry_count);

    Eco_EConnect_Instance_OptionallyBindObject(reader->instance, (struct Eco_Object*) the_interface, id);

    for (index = 0; index < entry_count; index++)
    {
        if (!Eco_EConnect_Reader_ReadObject(reader, result, (struct Eco_Object**) &entry.key))
            return false;
        if (!Eco_EConnect_Reader_ReadObject(reader, result, (struct Eco_Object**) &entry.return_type))
            return false;
        entry.has_varargs = Eco_EConnect_ParseUInt(&reader->stream) != 0;
        entry.arg_count = Eco_EConnect_ParseUInt(&reader->stream);
        for (arg = 0; arg < entry.arg_count; arg++)
        {
            if (!Eco_EConnect_Reader_ReadObject(reader, result, (struct Eco_Object**) &entry.arg_types[arg]))
                return false;
        }
        the_interface->entries[index] = entry;
    }

    Eco_EConnect_Result_Create_Object(result, (struct Eco_Object*) the_interface);

    return true;
}

bool Eco_EConnect_Builtin_GetDefaultInterface(struct Eco_EConnect_Reader* reader,
                                              struct Eco_EConnect_Result* result)
{
    Eco_EConnect_Result_Create_Object(result, (struct Eco_Object*) Eco_Interface_GetDefaultInterface());
    return true;
}
