#include "group.h"

#include <ecore/io/econnect/reader/parser.h>
#include <ecore/objects/group/group.h>


bool Eco_EConnect_Builtin_GetGroup(struct Eco_EConnect_Reader* reader,
                                   struct Eco_EConnect_Result* result)
{
    unsigned int        i;
    unsigned int        id;
    unsigned int        count;
    struct Eco_Group*   the_group;
    struct Eco_Object*  object;

    the_group = Eco_Group_New();
    id        = Eco_EConnect_ParseUInt(&reader->stream);

    Eco_EConnect_Instance_OptionallyBindObject(reader->instance, (struct Eco_Object*) the_group, id);

    count     = Eco_EConnect_ParseUInt(&reader->stream);

    for (i = 0; i < count; i++)
    {
        if (!Eco_EConnect_Reader_ReadObject(reader, result, &object))
            return false;
        Eco_Group_AddObject(the_group, object);
    }

    Eco_EConnect_Result_Create_Object(result, (struct Eco_Object*) the_group);

    return true;
}
