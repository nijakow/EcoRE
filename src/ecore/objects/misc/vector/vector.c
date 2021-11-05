#include "vector.h"

#include <ecore/objects/base/type.h>
#include <ecore/vm/memory/gc/gc.h>
#include <ecore/vm/core/send.h>


/*
 *    T y p e C o r e
 */

static struct Eco_TypeCore Eco_Vector_TYPECORE;
static struct Eco_Type*    Eco_Vector_TYPE;
       struct Eco_Object*  Eco_Vector_PROXY = NULL;

bool Eco_Vector_Send(struct Eco_Message* message,
                     struct Eco_SendLink* link,
                     struct Eco_Vector* vector)
{
    if (Eco_Vector_PROXY != NULL)
        return Eco_Send_ToObject(message, link, Eco_Vector_PROXY);
    else
        return false;
}


void Eco_Vector_Init()
{
    Eco_TypeCore_Create(&Eco_Vector_TYPECORE, "Eco_Vector");

    Eco_Vector_TYPECORE.send = (Eco_TypeCore_SendFunc) Eco_Vector_Send;
    Eco_Vector_TYPECORE.mark = (Eco_TypeCore_MarkFunc) Eco_Vector_Mark;
    Eco_Vector_TYPECORE.del  = (Eco_TypeCore_DelFunc)  Eco_Vector_Del;

    Eco_Vector_TYPE          = Eco_Type_NewPrefab(&Eco_Vector_TYPECORE);
}

void Eco_Vector_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_Vector_TYPECORE);
}


/*
 *    B a s i c s
 */

struct Eco_Vector* Eco_Vector_New(unsigned int element_count)
{
    struct Eco_Vector*  vector;
    unsigned int        index;

    vector                = Eco_Object_New(Eco_Vector_TYPE,
                                           sizeof(struct Eco_Vector) + element_count * sizeof(Eco_Any),
                                           0);

    vector->element_count = element_count;

    for (index = 0; index < element_count; index++)
    {
        Eco_Any_AssignPointer(&vector->elements[index], (struct Eco_Object*) vector);
    }

    return vector;
}

void Eco_Vector_Mark(struct Eco_GC_State* state, struct Eco_Vector* vector)
{
    unsigned int  index;

    for (index = 0; index < vector->element_count; index++)
        Eco_GC_State_MarkAny(state, &vector->elements[index]);
    Eco_Object_Mark(state, &vector->_);
}

void Eco_Vector_Del(struct Eco_Vector* vector)
{
    Eco_Object_Del(&vector->_);
}
