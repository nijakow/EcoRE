#ifndef ECO_VM_CORE_SEND_H
#define ECO_VM_CORE_SEND_H

#include <ecore/eco.h>

struct Eco_Fiber;


enum Eco_Message_Type
{
    Eco_Message_Type_SEND,
    Eco_Message_Type_ASSIGN,
    Eco_Message_Type_AS
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
        struct {
            Eco_Any        value;
        } as;
    }                      body;
    struct Eco_Fiber*      fiber;
    struct Eco_Key*        key;
    enum Eco_Message_Type  type;
    // Set by Eco_Send(...)
    bool                   private_send;
};

bool Eco_Send_ToObject(struct Eco_Message*, struct Eco_Object*, Eco_Any, bool);
bool Eco_Send(struct Eco_Message*, Eco_Any, Eco_Any, bool, bool);

#endif
