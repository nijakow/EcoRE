#ifndef ECO_UTIL_DWARF_DWARF_H
#define ECO_UTIL_DWARF_DWARF_H

#include <ecore/base/defs.h>
#include <ecore/base/config.h>
#include <ecore/base/any.h>
#include <ecore/objects/vm/ffi/ffilib.h>

struct Eco_FFIType;
struct Eco_FFILib;

#ifdef ECO_CONFIG_USE_LIBDWARF

#include ECO_CONFIG_LIBDWARF_INCLUDE1
#include ECO_CONFIG_LIBDWARF_INCLUDE2


struct Eco_DwarfDie;

struct Eco_Dwarf
{
    int                   fd;
    Dwarf_Debug           debug;
    struct Eco_DwarfDie*  head;
    struct Eco_DwarfDie*  dies;
};

bool Eco_Dwarf_Create(struct Eco_Dwarf*, const char*);
void Eco_Dwarf_Destroy(struct Eco_Dwarf*);
void Eco_Dwarf_ExportInfo(struct Eco_Dwarf*, struct Eco_FFILib*);

struct Eco_DwarfDie* Eco_Dwarf_GetFirstDie(struct Eco_Dwarf*);


struct Eco_DwarfDie
{
    struct Eco_Dwarf*     session;
    Dwarf_Die             die;
    struct Eco_DwarfDie*  parent;
    struct Eco_DwarfDie*  sibling;
    struct Eco_DwarfDie*  child;
    struct Eco_DwarfDie** holder;
    struct Eco_DwarfDie** prev;
    struct Eco_DwarfDie*  next;
    struct Eco_FFIType*   ffi_type;
};

struct Eco_DwarfDie* Eco_DwarfDie_Sibling(struct Eco_DwarfDie*);
struct Eco_DwarfDie* Eco_DwarfDie_Child(struct Eco_DwarfDie*);
void Eco_DwarfDie_Delete(struct Eco_DwarfDie*);

bool Eco_DwarfDie_Is(struct Eco_DwarfDie*, const char*);
bool Eco_DwarfDie_Name(struct Eco_DwarfDie*, char*, Eco_Size_t);
bool Eco_DwarfDie_AttrName(struct Eco_DwarfDie*, char*, Eco_Size_t);
bool Eco_DwarfDie_AttrType(struct Eco_DwarfDie*, struct Eco_DwarfDie**);
bool Eco_DwarfDie_AttrLocation(struct Eco_DwarfDie*, void**);
bool Eco_DwarfDie_AttrUpperBound(struct Eco_DwarfDie*, long*);
bool Eco_DwarfDie_AttrByteSize(struct Eco_DwarfDie*, Eco_Size_t*);
bool Eco_DwarfDie_AttrExternal(struct Eco_DwarfDie*, bool*);


#endif

bool Eco_Dwarf_LoadDebugInfo(const char*, struct Eco_FFILib*);

#endif

