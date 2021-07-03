#include "reader.h"


void Eco_EConnect_Reader_Create(struct Eco_EConnect_Reader* reader,
                                struct Eco_EConnect_Instance* instance,
                                char* buffer,
                                unsigned int bufsize,
                                void (*del)(u8*))
{
    reader->instance = instance;
    Eco_IO_ByteInputStream_Create(&(reader->bytes), buffer, bufsize, del);
}

void Eco_EConnect_Reader_Destroy(struct Eco_EConnect_Reader* reader)
{
    Eco_IO_ByteInputStream_Destroy(&(reader->bytes));
}

