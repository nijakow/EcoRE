#include "parser.h"

#include "../../objects/key.h"
#include "../../vm/memory/memory.h"


void Eco_EConnect_ParseBytes(struct Eco_IO_ByteStream* stream,
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


struct Eco_Key* Eco_EConnect_ParseKey(struct Eco_IO_ByteStream* stream)
{
    unsigned int  bytes;

    bytes = Eco_EConnect_ParseUInt(stream);

    char          buffer[bytes + 1];
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

