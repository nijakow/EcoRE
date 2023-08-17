#ifndef ECO_IO_STREAMS_BYTESTREAM_H
#define ECO_IO_STREAMS_BYTESTREAM_H

#include "../../eco.h"


struct Eco_IO_ByteInputStream
{
    char*         bytes;
    unsigned int  index;
    unsigned int  count;
    void          (*del)(char*);
};

static inline void Eco_IO_ByteInputStream_Create(struct Eco_IO_ByteInputStream* bytestream,
                                                 char* bytes,
                                                 unsigned int count,
                                                 void (*del)(char*))
{
    bytestream->bytes = bytes;
    bytestream->index = 0;
    bytestream->count = count;
    bytestream->del   = del;
}

static inline void Eco_IO_ByteInputStream_Destroy(struct Eco_IO_ByteInputStream* bytestream)
{
    if (bytestream->del != NULL) {
        bytestream->del(bytestream->bytes);
    }
}

static inline bool Eco_IO_ByteInputStream_HasMore(struct Eco_IO_ByteInputStream* bytestream)
{
    return bytestream->index < bytestream->count;
}

static inline u8 Eco_IO_ByteInputStream_Peek(struct Eco_IO_ByteInputStream* bytestream)
{
    return bytestream->bytes[bytestream->index];
}

static inline u8 Eco_IO_ByteInputStream_Read(struct Eco_IO_ByteInputStream* bytestream)
{
    if (Eco_IO_ByteInputStream_HasMore(bytestream)) {
        return bytestream->bytes[bytestream->index++];
    } else {
        return 0x00;
    }
}

#endif
