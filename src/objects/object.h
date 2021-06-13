#pragma once

#include "../eco.h"


struct Eco_Message;
struct Eco_GC_State;


/*
 *
 *    s t r u c t   E c o _ O b j e c t
 *
 */

struct Eco_Object
{
    struct Eco_Object*  next;
    struct Eco_Type*    type;
    struct {
        unsigned int mark_queued : 1;
        unsigned int mark_done   : 1;
    }                   header;
};

static inline void* Eco_Object_At(struct Eco_Object* object, unsigned int offset)
{
    return (void*) (((char*) object) + sizeof(struct Eco_Object) + offset);
}


void* Eco_Object_New(struct Eco_Type*, unsigned int);
bool Eco_Object_Send(struct Eco_Message*, struct Eco_Object*);
void Eco_Object_Mark(struct Eco_GC_State*, struct Eco_Object*);
void Eco_Object_Del(struct Eco_Object*);
