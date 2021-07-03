#pragma once

#include "econnect.h"
#include "../../eco.h"


unsigned int Eco_EConnect_ParseUInt(struct Eco_IO_ByteInputStream* stream);
void Eco_EConnect_ParseBytes(struct Eco_IO_ByteInputStream*, char*, unsigned int);
struct Eco_Key* Eco_EConnect_ParseKey(struct Eco_IO_ByteInputStream*);
struct Eco_Object* Eco_EConnect_ParseObject_ByID(struct Eco_EConnect_Reader*);

