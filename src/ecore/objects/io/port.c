#include "port.h"

#include <ecore/objects/base/type.h>

/*
 *    T y p e C o r e
 */

static struct Eco_TypeCore Eco_Port_TYPECORE;
       struct Eco_Type*    Eco_Port_TYPE;


void Eco_Port_Init()
{
    Eco_TypeCore_Create(&Eco_Port_TYPECORE, "Eco_Port");

    Eco_Port_TYPECORE.send = (Eco_TypeCore_SendFunc) Eco_Object_Send;
    Eco_Port_TYPECORE.mark = (Eco_TypeCore_MarkFunc) Eco_Port_Mark;
    Eco_Port_TYPECORE.del  = (Eco_TypeCore_DelFunc)  Eco_Port_Del;

    Eco_Port_TYPE          = Eco_Type_NewPrefab(&Eco_Port_TYPECORE);
}

void Eco_Port_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_Port_TYPECORE);
}


/*
 *    B a s i c s
 */

struct Eco_Port* Eco_Port_New()
{
    struct Eco_Port*  port;

    port = Eco_Object_New(Eco_Port_TYPE, sizeof(struct Eco_Port), 0);

    if (port != NULL) {

    }

    return port;
}

void Eco_Port_Mark(struct Eco_GC_State* state, struct Eco_Port* port)
{
    Eco_Object_Mark(state, &port->_);
}

void Eco_Port_Del(struct Eco_Port* port)
{
    Eco_Object_Del(&port->_);
}
