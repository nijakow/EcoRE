#ifndef ECO_IO_ECONNECT_READER_PARSER_H
#define ECO_IO_ECONNECT_READER_PARSER_H

#include "../../../eco.h"

#include "../../streams/bytestream.h"

unsigned char Eco_EConnect_ParseByte(struct Eco_IO_ByteInputStream*);
unsigned int Eco_EConnect_ParseUInt(struct Eco_IO_ByteInputStream*);
void Eco_EConnect_ParseBytes(struct Eco_IO_ByteInputStream*, char*, unsigned int);
struct Eco_Key* Eco_EConnect_ParseKey(struct Eco_IO_ByteInputStream*);

#endif
