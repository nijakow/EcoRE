#pragma once

#include "../../eco.h"


struct Eco_IO_ByteStream
{
    u8*           bytes;
    unsigned int  index;
    unsigned int  count;
    void          (*del)(u8*);
};

static inline void Eco_IO_ByteStream_Create(struct Eco_IO_ByteStream* bytestream,
                                            u8* bytes,
                                            unsigned int count,
                                            void (*del)(u8*))
{
    bytestream->bytes = bytes;
    bytestream->index = 0;
    bytestream->count = count;
    bytestream->del   = del;
}

static inline void Eco_IO_ByteStream_Destroy(struct Eco_IO_ByteStream* bytestream)
{
    if (bytestream->del != NULL) {
        bytestream->del(bytestream->bytes);
    }
}

static inline bool Eco_IO_ByteStream_HasMore(struct Eco_IO_ByteStream* bytestream)
{
    return bytestream->index < bytestream->count;
}

static inline u8 Eco_IO_ByteStream_Peek(struct Eco_IO_ByteStream* bytestream)
{
    return bytestream->bytes[bytestream->index];
}

static inline u8 Eco_IO_ByteStream_Read(struct Eco_IO_ByteStream* bytestream)
{
    if (Eco_IO_ByteStream_HasMore(bytestream)) {
        return bytestream->bytes[bytestream->index++];
    } else {
        return 0x00;
    }
}

