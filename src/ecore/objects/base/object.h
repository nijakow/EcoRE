#ifndef ECO_OBJECTS_BASE_OBJECT_H
#define ECO_OBJECTS_BASE_OBJECT_H

#include "../../eco.h"

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


struct Eco_Object* Eco_Object_New();
void* Eco_Object_New_Derived(struct Eco_Type*, unsigned int, unsigned int);
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
