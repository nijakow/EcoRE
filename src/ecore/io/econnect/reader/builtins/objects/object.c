#include "object.h"

#include "../../parser.h"

#include "../../../../../objects/base/object.h"
#include "../../../../../objects/base/slot.h"


bool Eco_EConnect_Builtin_GetObject_Parse(struct Eco_EConnect_Reader* reader,
                                          struct Eco_EConnect_Result* result,
                                          struct Eco_Object* object)
{
    unsigned int                slot_def_current;
    unsigned int                slot_def_max;
    unsigned char               flags;
    struct Eco_Object_SlotInfo  slot_info;
    Eco_Any                     any;

    slot_def_max = Eco_EConnect_ParseUInt(&reader->stream);

    for (slot_def_current = 0;
         slot_def_current < slot_def_max;
         slot_def_current++)
    {
        flags = Eco_EConnect_ParseByte(&reader->stream);

        if (!Eco_EConnect_Reader_Read(reader, result)
            || !Eco_EConnect_Result_ExpectObject(result, &slot_info.key)) {
            return false;
        }

        if (flags & 0x01) {
            slot_info.is_inherited = flags & 0x02;
            if (!Eco_EConnect_Reader_ReadAny(reader, result, &any)) {
                return false;
            }
            Eco_Object_AddSlot(object, -1, slot_info, &any);
        } else {
            slot_info.is_inherited = false;
        }
    }

    return true;
}


bool Eco_EConnect_Builtin_GetObject(struct Eco_EConnect_Reader* reader,
                                    struct Eco_EConnect_Result* result)
{
    unsigned int        id;
    struct Eco_Object*  the_object;

    id         = Eco_EConnect_ParseUInt(&reader->stream);
    the_object = Eco_Object_New();

    Eco_EConnect_Instance_OptionallyBindObject(reader->instance, the_object, id);
    if (!Eco_EConnect_Builtin_GetObject_Parse(reader, result, the_object))
        return false;
    Eco_EConnect_Result_Create_Object(result, the_object);

    return true;
}
