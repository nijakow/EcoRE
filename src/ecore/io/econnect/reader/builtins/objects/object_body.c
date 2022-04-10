#include "object_body.h"

#include <ecore/io/econnect/reader/parser.h>
#include <ecore/objects/base/type/slot_info.h>
#include <ecore/objects/base/molecule/object_slots.h>
#include <ecore/objects/misc/key/key.h>
#include <ecore/objects/vm/interface/interface.h>


bool Eco_EConnect_Reader_ReadMoleculeBody(struct Eco_EConnect_Reader* reader,
                                          struct Eco_EConnect_Result* result,
                                          struct Eco_Molecule* object)
{
    unsigned int              ups;
    unsigned int              slot_def_current;
    unsigned int              slot_def_max;
    unsigned char             flags;
    struct Eco_BasicSlotInfo  slot_info;
    Eco_Any                   any;
    struct Eco_Code*          code;

    ups = Eco_EConnect_ParseUInt(&reader->stream);

    while (ups --> 0)
    {
        if (!Eco_EConnect_Reader_Read(reader, result))
            return false;
        if (!Eco_EConnect_Result_ExpectObject(result, &object->_.up)) {
            Eco_EConnect_Result_Destroy(result);
            Eco_EConnect_Result_Create_Error(result, Eco_EConnect_ErrorType_TYPE_ERROR);
            return false;
        }
    }

    slot_def_max = Eco_EConnect_ParseUInt(&reader->stream);

    for (slot_def_current = 0;
         slot_def_current < slot_def_max;
         slot_def_current++)
    {
        Eco_BasicSlotInfo_Create(&slot_info);
        
        flags = Eco_EConnect_ParseByte(&reader->stream);

        if (!Eco_EConnect_Reader_Read(reader, result))
            return false;

        if (!Eco_EConnect_Result_ExpectObject(result, (struct Eco_Object**) &slot_info.key)) {
            Eco_EConnect_Result_Destroy(result);
            Eco_EConnect_Result_Create_Error(result, Eco_EConnect_ErrorType_TYPE_ERROR);
            return false;
        }

        if ((flags & 0x01) == 0) {
            slot_info.flags.is_with      =  (flags & 0x04) != 0;
            slot_info.flags.is_inherited =  (flags & 0x08) != 0;   // The flag 0x08 indicates "no delegate"
            slot_info.flags.is_part      =  (flags & 0x10) != 0;
            slot_info.flags.is_private   =  (flags & 0x20) != 0;
            if (flags & 0x02) {
                if (!Eco_EConnect_Reader_ReadAny(reader, result, &any))
                    return false;
            } else {
                /*
                 * No value was given by default, so we make the slot point
                 * to the object itself.
                 */
                any = Eco_Any_FromPointer(object);
            }
            Eco_Molecule_AddSlot(object, -1, &slot_info, Eco_Interface_GetDefaultInterface(), any); // TODO, FIXME, XXX: Parse the interface!
        } else {
            slot_info.flags.is_private = (flags & 0x20) != 0;
            if (!Eco_EConnect_Reader_ReadObject(reader, result, (struct Eco_Object**) &code))
                return false;
            Eco_Molecule_AddCodeSlot(object, -1, &slot_info, Eco_Interface_GetDefaultInterface(), code); // TODO, FIXME, XXX: Parse the interface!
        }
    }

    return true;
}
