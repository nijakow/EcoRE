#ifndef ECO_IO_ECONNECT_READER_CALLBACK_H
#define ECO_IO_ECONNECT_READER_CALLBACK_H

#include <ecore/eco.h>

#include "reader.h"
#include "result.h"


struct Eco_EConnect_Reader;

typedef bool (*Eco_EConnect_Callback)(struct Eco_EConnect_Reader*, struct Eco_EConnect_Result*);

void Eco_EConnect_InstallCallback(char*, Eco_EConnect_Callback);

#endif
