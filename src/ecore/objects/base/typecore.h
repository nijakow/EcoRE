#ifndef ECO_OBJECTS_BASE_TYPECORE_H
#define ECO_OBJECTS_BASE_TYPECORE_H

#include <ecore/eco.h>

struct Eco_Object;
struct Eco_Message;
struct Eco_SendLink;
struct Eco_GC_State;
struct Eco_CloneState;


typedef bool (*Eco_TypeCore_SendFunc)(struct Eco_Message*, struct Eco_SendLink*, struct Eco_Object*, Eco_Any*);
typedef void (*Eco_TypeCore_MarkFunc)(struct Eco_GC_State*, struct Eco_Object*);
typedef struct Eco_Object* (*Eco_TypeCore_CloneFunc)(struct Eco_CloneState*, struct Eco_Object*);
typedef void (*Eco_TypeCore_DelFunc)(struct Eco_Object*);


struct Eco_TypeCore
{
    Eco_TypeCore_SendFunc   send;    
    Eco_TypeCore_MarkFunc   mark;
    Eco_TypeCore_CloneFunc  clone;
    Eco_TypeCore_DelFunc    del;

    const char*             name;
};


void Eco_TypeCore_Create(struct Eco_TypeCore*, const char*);
void Eco_TypeCore_Destroy(struct Eco_TypeCore*);

#endif
