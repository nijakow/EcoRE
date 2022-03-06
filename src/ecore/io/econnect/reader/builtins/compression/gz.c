#include "gz.h"

#include <ecore/io/econnect/reader/parser.h>
#include <ecore/vm/memory/memory.h>
#include <ecore/util/gz.h>


bool Eco_EConnect_Builtin_GZip(struct Eco_EConnect_Reader* reader,
                               struct Eco_EConnect_Result* result)
{
    struct Eco_EConnect_Reader  subreader;
    unsigned int                compressed_length;
    unsigned int                uncompressed_length;
    char*                       bytes;
    bool                        retval;

    compressed_length   = Eco_EConnect_ParseUInt(&reader->stream);
    uncompressed_length = Eco_EConnect_ParseUInt(&reader->stream);

    bytes               = Eco_Memory_Alloc(compressed_length + uncompressed_length);

    if (bytes == NULL) {
        Eco_EConnect_Result_Create_Error(result, Eco_EConnect_Result_ErrorType_ALLOCATION_ERROR);
        return false;
    }

    Eco_EConnect_ParseBytes(&reader->stream, bytes, compressed_length);

    Eco_GZ_Uncompress(&bytes[compressed_length], uncompressed_length,
                      &bytes[0], compressed_length);

    Eco_EConnect_Reader_Create(&subreader, reader->instance, &bytes[compressed_length], uncompressed_length, NULL);

    retval = Eco_EConnect_Reader_Read(&subreader, result);

    Eco_Memory_Free(bytes);

    return retval;
}
