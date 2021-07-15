#pragma once

#include "../instance.h"
#include "result.h"

#include "../../streams/bytestream.h"


struct Eco_EConnect_Reader
{
    struct Eco_IO_ByteInputStream  stream;
    struct Eco_EConnect_Instance*  instance;
};

void  Eco_EConnect_Reader_Create(struct Eco_EConnect_Reader*,
                                 struct Eco_EConnect_Instance*,
                                 char*,
                                 unsigned int,
                                 void (*del)(char*));
void  Eco_EConnect_Reader_Destroy(struct Eco_EConnect_Reader*);

struct Eco_Object* Eco_EConnect_Reader_GetObjectByID(struct Eco_EConnect_Reader*, unsigned int);
bool Eco_EConnect_Reader_ReadAny(struct Eco_EConnect_Reader*, struct Eco_EConnect_Result*, Eco_Any*);
bool Eco_EConnect_Reader_Read(struct Eco_EConnect_Reader*, struct Eco_EConnect_Result*);
