#pragma once

#include "../../eco.h"


struct Eco_IO_StringStream
{
    char*  begin;
    char*  current;
    char*  end;
};

static inline bool Eco_IO_StringStream_HasMore(struct Eco_IO_StringStream* stream)
{
    return stream->current < stream->end;
}

static inline char Eco_IO_StringStream_PeekChar(struct Eco_IO_StringStream* stream)
{
    return *(stream->current);
}

static inline char Eco_IO_StringStream_NextChar(struct Eco_IO_StringStream* stream)
{
    return *(stream->current++);
}

