#ifndef ECO_OBJECTS_MISC_VECTOR_VECTOR_H
#define ECO_OBJECTS_MISC_VECTOR_VECTOR_H

#include <ecore/eco.h>

#include <ecore/objects/base/object.h>


struct Eco_Vector_Payload
{
    unsigned int  alloc;
    unsigned int  fill;
    Eco_Any       elements[];
};

struct Eco_Vector
{
    struct Eco_Object           _;
    struct Eco_Vector_Payload*  payload;
};

struct Eco_Vector* Eco_Vector_New(unsigned int);
void Eco_Vector_Mark(struct Eco_GC_State*, struct Eco_Vector*);
void Eco_Vector_Del(struct Eco_Vector*);

static inline unsigned int Eco_Vector_Size(struct Eco_Vector* vector)
{
    return vector->payload->fill;
}

static inline Eco_Any* Eco_Vector_At(struct Eco_Vector* vector, unsigned int index)
{
    return &vector->payload->elements[index];
}

static inline void Eco_Vector_Put(struct Eco_Vector* vector, unsigned int index, Eco_Any* value)
{
    Eco_Any_AssignAny(&vector->payload->elements[index], value);
}

bool Eco_Vector_Insert(struct Eco_Vector* vector, unsigned int index, Eco_Any* value);
bool Eco_Vector_Remove(struct Eco_Vector* vector, unsigned int index);

void Eco_Vector_Init();
void Eco_Vector_Terminate();

#endif
