#include "object_body.h"

#include "../../parser.h"
#include "../../../../../objects/base/object_slots.h"
#include "../../../../../objects/base/slot_info.h"


bool Eco_EConnect_Reader_ReadObjectBody(struct Eco_EConnect_Reader* reader,
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

        if (!Eco_EConnect_Reader_Read(reader, result))
            return false;

        if (!Eco_EConnect_Result_ExpectObject(result, &slot_info.key)) {
            Eco_EConnect_Result_Destroy(result);
            Eco_EConnect_Result_Create_Error(result, Eco_EConnect_ErrorType_TYPE_ERROR);
            return false;
        }

        if ((flags & 0x01) == 0) {
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
