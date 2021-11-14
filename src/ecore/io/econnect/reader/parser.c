#include "parser.h"

#include <ecore/objects/misc/key/key.h>


unsigned char Eco_EConnect_ParseByte(struct Eco_IO_ByteInputStream* stream)
{
    return (unsigned char) Eco_IO_ByteInputStream_Read(stream);
}

unsigned int Eco_EConnect_ParseUInt(struct Eco_IO_ByteInputStream* stream)
{
    u8            byte;
    unsigned int  value;

    value = 0;
    do
    {
        byte = Eco_IO_ByteInputStream_Read(stream);
        value = (value << 7) | (byte & 0b01111111);
    } while ((byte & 0b10000000) != 0);

    return value;
}

int Eco_EConnect_ParseInt(struct Eco_IO_ByteInputStream* stream)
{
    u8    byte;
    int   value;
    int   factor;

    if (Eco_IO_ByteInputStream_Peek(stream) == 0x80) {
        factor = -1;
        Eco_IO_ByteInputStream_Read(stream);
    } else {
        factor = 1;
    }

    value = 0;
    do
    {
        byte = Eco_IO_ByteInputStream_Read(stream);
        value = (value << 7) | (byte & 0b01111111);
    } while ((byte & 0b10000000) != 0);

    return value * factor;
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
}

void Eco_EConnect_ParseString(struct Eco_IO_ByteInputStream* stream,
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
    Eco_EConnect_ParseString(stream, buffer, bytes);

    return Eco_Key_Find(buffer);
}
