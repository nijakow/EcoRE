#include "parser.h"

#include "../../objects/key.h"
#include "../../vm/memory/memory.h"


void Eco_EConnect_ParseBytes(struct Eco_IO_ByteInputStream* stream,
                             char* buffer,
                             unsigned int count)
{
    while (count > 0)
    {
        *buffer = Eco_IO_ByteInputStream_Read(stream);
        buffer++;
        count--;
    }
    *buffer = '\0';
}


struct Eco_Key* Eco_EConnect_ParseKey(struct Eco_IO_ByteInputStream* stream)
{
    unsigned int  bytes;

    bytes = Eco_EConnect_ParseUInt(stream);

    char          buffer[bytes + 1];
    Eco_EConnect_ParseBytes(stream, buffer, bytes);

    return Eco_Key_Find(buffer);
}

struct Eco_Object* Eco_EConnect_ParseObject_ByID(struct Eco_EConnect_Reader* reader)
{
    unsigned int  id;

    id = Eco_EConnect_ParseUInt(&(reader->bytes));

    if (id < reader->state->objects_by_id_max) {
        return reader->state->objects_by_id[id];
    } else {
        return NULL;
    }
}
