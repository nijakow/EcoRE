#pragma once

#include "econnect.h"

#include "../streams/bytestream.h"


struct Eco_EConnect_Reader
{
    struct Eco_IO_ByteInputStream  bytes;
    struct Eco_EConnect_State*     state;
};

void  Eco_EConnect_Reader_Create(struct Eco_EConnect_Reader*,
                                 struct Eco_EConnect_State*,
                                 char*,
                                 unsigned int,
                                 void (*del)(u8*));
void  Eco_EConnect_Reader_Destroy(struct Eco_EConnect_Reader*);

