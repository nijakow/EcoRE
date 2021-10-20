#ifndef ECO_VM_CORE_SEND_H
#define ECO_VM_CORE_SEND_H

#include <ecore/eco.h>

struct Eco_Fiber;


enum Eco_Message_Type
{
    Eco_Message_Type_SEND,
    Eco_Message_Type_ASSIGN
};

struct Eco_Message
{
    union {
        struct {
            unsigned int   arg_count;
        } send;
        struct {
            Eco_Any        value;
        } assign;
    }                      body;
    struct Eco_Fiber*      fiber;
    struct Eco_Object*     key;
    enum Eco_Message_Type  type;
};

struct Eco_SendLink
{
    struct Eco_SendLink*     previous;
    struct Eco_Object*   object;
};

bool Eco_Send_ToObject(struct Eco_Message*, struct Eco_SendLink*, struct Eco_Object*);
bool Eco_Send(struct Eco_Message*, struct Eco_SendLink*, Eco_Any*);

#endif
