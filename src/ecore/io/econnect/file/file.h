#ifndef ECO_IO_ECONNECT_FILE_FILE_H
#define ECO_IO_ECONNECT_FILE_FILE_H

#include "../reader/reader.h"
#include "../reader/result.h"

bool Eco_EConnect_ReadFile(const char*, struct Eco_EConnect_Result*);
bool Eco_EConnect_LoadImage(struct Eco_EConnect_Result*, char*, unsigned long);

#endif
