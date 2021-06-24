#include "econnect.h"

#include "../../objects/key.h"


static inline unsigned int Eco_EConnect_ParseUInt(struct Eco_IO_ByteStream* stream)
{
    u8            byte;
    unsigned int  value;

    value = 0;
    do
    {
        byte = Eco_IO_ByteStream_Read(stream);
        value = (value << 7) | (byte & 0b01111111);
    } while ((byte & 0b1000000) != 0);

    return value;
}

static void Eco_EConnect_ParseBytes(struct Eco_IO_ByteStream* stream,
                                    char* buffer,
                                    unsigned int count)
{
    while (count > 0)
    {
        *buffer = Eco_IO_ByteStream_Read(stream);
        buffer++;
        count--;
    }
    *buffer = '\0';
}


static struct Eco_Key* Eco_EConnect_ParseKey(struct Eco_IO_ByteStream* stream)
{
    unsigned int  bytes;

    bytes = Eco_EConnect_ParseUInt(stream);

    char           buffer[bytes + 1];
    Eco_EConnect_ParseBytes(stream, buffer, bytes);

    return Eco_Key_Find(buffer);
}

struct Eco_Object* Eco_EConnect_ParseObject_ByID(struct Eco_EConnect_Message* message)
{
    unsigned int  id;

    id = Eco_EConnect_ParseUInt(&(message->bytes));

    if (id < message->state->objects_by_id_max) {
        return message->state->objects_by_id[id];
    } else {
        return NULL;
    }
}

void* Eco_EConnect_Message_Process(struct Eco_EConnect_Message* message)
{
    struct Eco_Key*  msg_key;

    /* TODO: This cast is dangerous! */
    msg_key = (struct Eco_Key*) Eco_EConnect_ParseObject_ByID(message);

    if (msg_key->econnect_func != NULL) {
        return msg_key->econnect_func(message);
    } else {
        /* TODO: Error */
        return NULL;
    }
}
