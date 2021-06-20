#pragma once

#include "../../eco.h"

#include "../streams/string_stream.h"


struct Eco_EConnect_RefInfo;


struct Eco_EConnect
{
    struct Eco_EConnect_RefInfo*  infos;
};


struct Eco_EConnect_IStream
{
    struct Eco_EConnect*        connection;
    struct Eco_IO_StringStream  stream;
};

