#pragma once

#include "../streams/bytestream.h"


struct Eco_EConnect_State
{
    unsigned int         objects_by_id_max;
    struct Eco_Object**  objects_by_id;
};


struct Eco_EConnect_Message
{
    struct Eco_IO_ByteStream    bytes;
    struct Eco_EConnect_State*  state;
};

void* Eco_EConnect_Message_Process(struct Eco_EConnect_Message*);
