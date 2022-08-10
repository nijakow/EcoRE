#ifndef ECO_OBJECTS_VM_CODE_CODE_H
#define ECO_OBJECTS_VM_CODE_CODE_H

#include <ecore/eco.h>

#include <ecore/objects/base/object.h>

struct Eco_GC_State;
struct Eco_Blob;
struct Eco_Array;

struct Eco_Code
{
    struct Eco_Object  _;

    unsigned int       register_count;
    unsigned int       arg_count;
    bool               has_varargs;

    unsigned int       bytecode_count;
    u8*                bytecodes;
    
    unsigned int       constant_count;
    Eco_Any*           constants;
};


struct Eco_Code* Eco_Code_New();
void Eco_Code_Mark(struct Eco_GC_State*, struct Eco_Code*);
void Eco_Code_Del(struct Eco_Code*);

struct Eco_Code* Eco_Code_ConstructFromEco(struct Eco_Blob*,
                                           struct Eco_Array*,
                                           unsigned int,
                                           unsigned int,
                                           bool);

void Eco_Code_Init();
void Eco_Code_Terminate();

#endif
