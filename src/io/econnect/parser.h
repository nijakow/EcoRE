#pragma once

#include "econnect.h"
#include "../../eco.h"


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

void Eco_EConnect_ParseBytes(struct Eco_IO_ByteStream*, char*, unsigned int);
struct Eco_Key* Eco_EConnect_ParseKey(struct Eco_IO_ByteStream*);
struct Eco_Object* Eco_EConnect_ParseObject_ByID(struct Eco_EConnect_Message*);
