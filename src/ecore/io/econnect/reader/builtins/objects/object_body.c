#include "object_body.h"

#include <ecore/io/econnect/reader/parser.h>
#include <ecore/objects/base/object_slots.h>
#include <ecore/objects/base/slot_info.h>
#include <ecore/objects/misc/key/key.h>


bool Eco_EConnect_Reader_ReadMoleculeBody(struct Eco_EConnect_Reader* reader,
                                          struct Eco_EConnect_Result* result,
                                          struct Eco_Molecule* object)
{
    unsigned int                slot_def_current;
    unsigned int                slot_def_max;
    unsigned char               flags;
    struct Eco_Object_SlotInfo  slot_info;
    Eco_Any                     any;
    struct Eco_Code*            code;

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
            slot_info.is_inherited = (flags & 0x04) != 0;
            slot_info.is_part      = (flags & 0x08) != 0;
            if (flags & 0x02) {
                if (!Eco_EConnect_Reader_ReadAny(reader, result, &any))
                    return false;
            } else {
                /*
                 * No value was given by default, so we make the slot point
                 * to the object itself.
                 */
                Eco_Any_AssignPointer(&any, (struct Eco_Object*) object);
            }
            Eco_Molecule_AddSlot(object, -1, slot_info, &any);
        } else {
            slot_info.is_inherited = false;
            slot_info.is_part      = false;
            if (!Eco_EConnect_Reader_ReadObject(reader, result, (struct Eco_Object**) &code))
                return false;
            Eco_Molecule_AddCodeSlot(object, -1, slot_info, code);
        }
    }

    return true;
}
