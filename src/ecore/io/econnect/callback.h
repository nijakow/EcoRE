#pragma once

#include "../../eco.h"

struct Eco_EConnect_Reader;
struct Eco_EConnect_ParseResult;

typedef bool (*Eco_EConnect_Callback)(struct Eco_EConnect_Reader*, struct Eco_EConnect_ParseResult*);

void Eco_EConnect_InstallCallback(char*, Eco_EConnect_Callback);
