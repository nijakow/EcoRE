#ifndef ECO_OBJECTS_MISC_VECTOR_VECTOR_H
#define ECO_OBJECTS_MISC_VECTOR_VECTOR_H

#include <ecore/eco.h>

#include <ecore/objects/base/object.h>


struct Eco_Vector
{
    struct Eco_Object  _;
    unsigned int       element_count;
    Eco_Any            elements[];
};

struct Eco_Vector* Eco_Vector_New(unsigned int);
void Eco_Vector_Mark(struct Eco_GC_State*, struct Eco_Vector*);
void Eco_Vector_Del(struct Eco_Vector*);

static inline void Eco_Vector_Put(struct Eco_Vector* vector, unsigned int index, Eco_Any* value)
{
    Eco_Any_AssignAny(&vector->elements[index], value);
}

void Eco_Vector_Init();
void Eco_Vector_Terminate();

#endif
