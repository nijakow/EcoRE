#ifndef ECO_OBJECTS_BASE_OBJECT_H
#define ECO_OBJECTS_BASE_OBJECT_H

#include <ecore/eco.h>
#include <ecore/vm/memory/arena.h>

#include "object_payload.h"
#include "slot_info.h"


struct Eco_Message;
struct Eco_GC_State;
struct Eco_Code;


/*
 *
 *    s t r u c t   E c o _ O b j e c t
 *
 */

struct Eco_Object_Header
{
    unsigned int mark_queued : 1;
    unsigned int mark_done   : 1;
    unsigned int object_size : 30;
};

struct Eco_Object
{
    struct Eco_Object*          next;
    struct Eco_Type*            type;
    struct Eco_Object_Header    header;
    struct Eco_Object_Payload*  payload;
};


void* Eco_Object_New(struct Eco_Type*, unsigned int, unsigned int);
void* Eco_Object_NewInArena(struct Eco_Type*, unsigned int, unsigned int, struct Eco_Arena*);
struct Eco_Object* Eco_Object_NewPlain();

bool  Eco_Object_Send(struct Eco_Message*, struct Eco_Object*);
void  Eco_Object_Mark(struct Eco_GC_State*, struct Eco_Object*);
void  Eco_Object_Del(struct Eco_Object*);

static inline void* Eco_Object_At(struct Eco_Object* object, unsigned int offset)
{
    return (void*) &(object->payload->data[offset]);
}

void Eco_Object_Init();
void Eco_Object_Terminate();

#endif
