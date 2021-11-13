#include "vector.h"

#include <ecore/objects/base/type.h>
#include <ecore/vm/memory/memory.h>
#include <ecore/vm/memory/gc/gc.h>
#include <ecore/vm/core/send.h>


/*
 *    T y p e
 */

static struct Eco_TypeCore Eco_Vector_TYPECORE;
       struct Eco_Type*    Eco_Vector_TYPE;


void Eco_Vector_Init()
{
    Eco_TypeCore_Create(&Eco_Vector_TYPECORE, "Eco_Vector");

    Eco_Vector_TYPECORE.send = (Eco_TypeCore_SendFunc) Eco_Object_Send;
    Eco_Vector_TYPECORE.mark = (Eco_TypeCore_MarkFunc) Eco_Vector_Mark;
    Eco_Vector_TYPECORE.del  = (Eco_TypeCore_DelFunc)  Eco_Vector_Del;

    Eco_Vector_TYPE          = Eco_Type_NewPrefab(&Eco_Vector_TYPECORE);
}

void Eco_Vector_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_Vector_TYPECORE);
}


/*
 *    P a y l o a d
 */

static struct Eco_Vector_Payload* Eco_Vector_Payload_New(unsigned int element_count)
{
    struct Eco_Vector_Payload*  payload;

    payload = Eco_Memory_Alloc(sizeof(struct Eco_Vector_Payload) + sizeof(Eco_Any) * element_count);

    if (payload != NULL) {
        payload->alloc = element_count;
        payload->fill  = 0;
    }

    return payload;
}

static void Eco_Vector_Payload_Delete(struct Eco_Vector_Payload* payload)
{
    Eco_Memory_Free(payload);
}


/*
 *    B a s i c s
 */

struct Eco_Vector* Eco_Vector_New(unsigned int element_count)
{
    struct Eco_Vector*  vector;

    vector          = Eco_Object_New(Eco_Vector_TYPE, sizeof(struct Eco_Vector), 0);
    vector->payload = Eco_Vector_Payload_New(element_count);

    return vector;
}

void Eco_Vector_Mark(struct Eco_GC_State* state, struct Eco_Vector* vector)
{
    unsigned int  index;

    if (vector->payload != NULL) {
        for (index = 0; index < vector->payload->fill; index++)
            Eco_GC_State_MarkAny(state, &vector->payload->elements[index]);
    }
    Eco_Object_Mark(state, &vector->_);
}

void Eco_Vector_Del(struct Eco_Vector* vector)
{
    Eco_Vector_Payload_Delete(vector->payload);
    Eco_Object_Del(&vector->_);
}


/*
 *    E l e m e n t   A c c e s s
 */

static bool Eco_Vector_ResizePayload(struct Eco_Vector* vector, unsigned int new_size)
{
    struct Eco_Vector_Payload* new_payload;
    
    new_payload = Eco_Memory_Realloc(vector->payload, sizeof(struct Eco_Vector_Payload) + sizeof(Eco_Any) * new_size);

    if (new_payload != NULL) {
        new_payload->alloc = new_size;
        vector->payload    = new_payload;
        return true;
    } else {
        return false;
    }
}

bool Eco_Vector_Insert(struct Eco_Vector* vector, unsigned int index, Eco_Any* value)
{
    unsigned int  element_index;

    if (vector->payload->fill + 1 >= vector->payload->alloc) {
        if (!Eco_Vector_ResizePayload(vector, vector->payload->fill + 1 + vector->payload->alloc / 2))
            return false;
    }

    vector->payload->fill += 1;
    for (element_index = vector->payload->fill - 1;
         element_index > index;
         element_index--)
    {
        Eco_Any_AssignAny(&vector->payload->elements[element_index],
                          &vector->payload->elements[element_index - 1]);
    }

    Eco_Any_AssignAny(&vector->payload->elements[index], value);

    return true;
}

bool Eco_Vector_Remove(struct Eco_Vector* vector, unsigned int index)
{
    vector->payload->fill--;
    while (index < vector->payload->fill)
    {
        Eco_Any_AssignAny(&vector->payload->elements[index],
                          &vector->payload->elements[index + 1]);
        index++;
    }
    return true;
}
