#pragma once

#include "../streams/bytestream.h"
#include "../../eco.h"


struct Eco_EConnect_State
{
    unsigned int         objects_by_id_max;
    struct Eco_Object**  objects_by_id;
};

void Eco_EConnect_State_Create(struct Eco_EConnect_State*);
void Eco_EConnect_State_Destroy(struct Eco_EConnect_State*);


struct Eco_EConnect_Message
{
    struct Eco_IO_ByteStream    bytes;
    struct Eco_EConnect_State*  state;
};

void  Eco_EConnect_Message_Create_ForReading(struct Eco_EConnect_Message*,
                                             struct Eco_EConnect_State*,
                                             char*,
                                             unsigned int,
                                             void (*del)(u8*));
void  Eco_EConnect_Message_Destroy(struct Eco_EConnect_Message*);


struct Eco_EConnect_ParseResult
{
};

bool Eco_EConnect_Message_Parse(struct Eco_EConnect_Message*, struct Eco_EConnect_ParseResult*);

