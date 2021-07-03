#include "reader.h"

#include "parser.h"

#include "../../objects/key.h"


void Eco_EConnect_Reader_Create(struct Eco_EConnect_Reader* reader,
                                struct Eco_EConnect_Instance* instance,
                                char* buffer,
                                unsigned int bufsize,
                                void (*del)(u8*))
{
    reader->instance = instance;
    Eco_IO_ByteInputStream_Create(&(reader->stream), buffer, bufsize, del);
}

void Eco_EConnect_Reader_Destroy(struct Eco_EConnect_Reader* reader)
{
    Eco_IO_ByteInputStream_Destroy(&(reader->stream));
}


bool Eco_EConnect_Reader_Read(struct Eco_EConnect_Reader* reader,
                              struct Eco_EConnect_Result* result)
{
    struct Eco_Key*  msg_key;

    result->type = Eco_EConnect_Result_Type_ERROR;  /* Initialize with an error */

    /* TODO, FIXME, XXX: This unchecked cast is dangerous! */
    msg_key = (struct Eco_Key*) Eco_EConnect_ParseObjectByID(reader);

    if (msg_key->econnect_callback != NULL) {
        return msg_key->econnect_callback(reader, result);
    } else {
        return false;
    }
}
