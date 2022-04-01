#ifndef ECO_OBJECTS_MISC_ARRAY_ARRAY_H
#define ECO_OBJECTS_MISC_ARRAY_ARRAY_H

#include <ecore/eco.h>

#include <ecore/objects/base/object.h>

struct Eco_String;

struct Eco_Array
{
    struct Eco_Object  _;
    unsigned int       size;
    Eco_Any            values[];
};

struct Eco_Array* Eco_Array_New(unsigned int);
void Eco_Array_Mark(struct Eco_GC_State*, struct Eco_Array*);
struct Eco_Array* Eco_Array_Clone(struct Eco_CloneState*, struct Eco_Array*, bool);
void Eco_Array_Del(struct Eco_Array*);

static inline unsigned int Eco_Array_Size(struct Eco_Array* array)
{
    return array->size;
}

static inline Eco_Any* Eco_Array_At(struct Eco_Array* array, unsigned int index)
{
    return &array->values[index];
}

static inline void Eco_Array_Put(struct Eco_Array* array, unsigned int index, Eco_Any value)
{
    array->values[index] = value;
}

struct Eco_String* Eco_Array_ToString(struct Eco_Array*);

void Eco_Array_Init();
void Eco_Array_Terminate();

#endif
