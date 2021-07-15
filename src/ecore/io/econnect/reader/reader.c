#include <stdio.h>

#include "reader.h"

#include "parser.h"

#include "builtins/objects/key.h"

#include "../../../objects/misc/key/key.h"


void Eco_EConnect_Reader_Create(struct Eco_EConnect_Reader* reader,
                                struct Eco_EConnect_Instance* instance,
                                char* buffer,
                                unsigned int bufsize,
                                void (*del)(char*))
{
    reader->instance = instance;
    Eco_IO_ByteInputStream_Create(&(reader->stream), buffer, bufsize, del);
}

void Eco_EConnect_Reader_Destroy(struct Eco_EConnect_Reader* reader)
{
    Eco_IO_ByteInputStream_Destroy(&(reader->stream));
}


struct Eco_Object* Eco_EConnect_Reader_GetObjectByID(struct Eco_EConnect_Reader* reader, unsigned int id)
{
    if (id < reader->instance->objects_by_id_max) {
        return reader->instance->objects_by_id[id];
    } else {
        return NULL;
    }
}

struct Eco_Object* Eco_EConnect_Reader_ReadObjectByID(struct Eco_EConnect_Reader* reader)
{
    return Eco_EConnect_Reader_GetObjectByID(reader, Eco_EConnect_ParseUInt(&reader->stream));
}

bool Eco_EConnect_Reader_ReadAny(struct Eco_EConnect_Reader* reader,
                                 struct Eco_EConnect_Result* result,
                                 Eco_Any* value)
{
    struct Eco_Object*  object;

    if (!Eco_EConnect_Reader_Read(reader, result)) {
        return false;
    } else if (Eco_EConnect_Result_ExpectObject(result, &object)) {
        Eco_Any_AssignPointer(value, object);
        return true;
    // TODO: Integers and Floats
    } else {
        Eco_EConnect_Result_Destroy(result);
        Eco_EConnect_Result_Create_Error(result, Eco_EConnect_ErrorType_EXPECTED_ANY);
        return false;
    }
}

bool Eco_EConnect_Reader_Read(struct Eco_EConnect_Reader* reader,
                              struct Eco_EConnect_Result* result)
{
    unsigned int     id;
    struct Eco_Key*  msg_key;

    Eco_EConnect_Result_Create_Error(result, Eco_EConnect_Result_ErrorType_UNDEFINED);

    /* TODO, FIXME, XXX: This unchecked cast is dangerous! */
    id = Eco_EConnect_ParseUInt(&reader->stream);
    if (id == 0x00) {
        if (!Eco_EConnect_Builtin_GetKey(reader, result))
            return false;
        // There shouldn't be any errors, GetKey(...) handles this
        Eco_EConnect_Result_ExpectObject(result, (struct Eco_Object**) &msg_key);
    } else {
        msg_key = (struct Eco_Key*) Eco_EConnect_Reader_GetObjectByID(reader, id);
    }

    if (msg_key != NULL && msg_key->econnect_callback != NULL) {
        return msg_key->econnect_callback(reader, result);
    } else {
        Eco_EConnect_Result_Create_Error(result, Eco_EConnect_Result_ErrorType_INVALID_MESSAGE_HEADER);
        return false;
    }
}
