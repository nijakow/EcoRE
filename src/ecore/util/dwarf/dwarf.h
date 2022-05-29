#ifndef ECO_UTIL_DWARF_DWARF_H
#define ECO_UTIL_DWARF_DWARF_H

#include <ecore/base/defs.h>
#include <ecore/base/config.h>

#ifdef ECO_CONFIG_USE_LIBDWARF

#include ECO_CONFIG_LIBDWARF_INCLUDE1
#include ECO_CONFIG_LIBDWARF_INCLUDE2

struct Eco_Dwarf
{
};

void Eco_Dwarf_Create(struct Eco_Dwarf*, const char*);
void Eco_Dwarf_Destroy(struct Eco_Dwarf*);

struct Eco_DwarfDie
{
    struct Eco_Dwarf*  session;
};

void Eco_DwarfDie_Delete(struct Eco_DwarfDie*);

bool Eco_DwarfDie_Is(struct Eco_DwarfDie*, const char*);

bool Eco_DwarfDie_AttrName(struct Eco_DwarfDie*, char*, Eco_Size_t);
bool Eco_DwarfDie_AttrType(struct Eco_DwarfDie*, struct Eco_DwarfDie**);
bool Eco_DwarfDie_AttrLocation(struct Eco_DwarfDie*, void**);
bool Eco_DwarfDie_AttrByteSize(struct Eco_DwarfDie*, Eco_Size_t*);
bool Eco_DwarfDie_AttrExternal(struct Eco_DwarfDie*, bool*);
// TODO: bool Eco_DwarfDie_AttrEncoding(struct Eco_DwarfDie*, enum Eco_DwarfEncoding);


#endif

#endif
