#include <zlib.h>

#include "gz.h"

bool Eco_GZ_Uncompress(char* dst, unsigned int dst_size, char* src, unsigned int src_size)
{
    z_stream  stream;

    stream.zalloc = Z_NULL;
    stream.zfree  = Z_NULL;
    stream.opaque = Z_NULL;

    stream.avail_in  = src_size;
    stream.next_in   = (Bytef*) src;
    stream.avail_out = dst_size;
    stream.next_out  = (Bytef*) dst;

    inflateInit(&stream);
    inflate(&stream, Z_NO_FLUSH);
    inflateEnd(&stream);

    return true;
}
