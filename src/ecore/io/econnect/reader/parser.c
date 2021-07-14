#include "parser.h"

#include "../../../objects/misc/key/key.h"


unsigned int Eco_EConnect_ParseUInt(struct Eco_IO_ByteInputStream* stream)
{
    u8            byte;
    unsigned int  value;

    value = 0;
    do
    {
        byte = Eco_IO_ByteInputStream_Read(stream);
        value = (value << 7) | (byte & 0b01111111);
    } while ((byte & 0b1000000) != 0);

    return value;
}

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
