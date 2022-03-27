#include "array.h"

#include <ecore/objects/base/type.h>
#include <ecore/objects/misc/string/string.h>
#include <ecore/vm/memory/memory.h>
#include <ecore/vm/memory/gc/gc.h>
#include <ecore/vm/core/clone.h>
#include <ecore/vm/core/send.h>
#include <ecore/util/utf8.h>


/*
 *    T y p e
 */

static struct Eco_TypeCore Eco_Array_TYPECORE;
       struct Eco_Type*    Eco_Array_TYPE;


void Eco_Array_Init()
{
    Eco_TypeCore_Create(&Eco_Array_TYPECORE, "Eco_Array");

    Eco_Array_TYPECORE.send  = (Eco_TypeCore_SendFunc)  Eco_Object_Send;
    Eco_Array_TYPECORE.mark  = (Eco_TypeCore_MarkFunc)  Eco_Array_Mark;
    Eco_Array_TYPECORE.clone = (Eco_TypeCore_CloneFunc) Eco_Array_Clone;
    Eco_Array_TYPECORE.del   = (Eco_TypeCore_DelFunc)   Eco_Array_Del;

    Eco_Array_TYPE           = Eco_Type_NewPrefab(&Eco_Array_TYPECORE);
}

void Eco_Array_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_Array_TYPECORE);
}


/*
 *    B a s i c s
 */

struct Eco_Array* Eco_Array_New(unsigned int element_count)
{
    struct Eco_Array*  array;
    unsigned int       i;
    Eco_Any            default_value;

    array = Eco_Object_New(Eco_Array_TYPE, sizeof(struct Eco_Array) + sizeof(Eco_Any) * element_count);

    if (array != NULL)
    {
        array->size = element_count;
        default_value = Eco_Any_FromPointer(array);
        for (i = 0; i < element_count; i++)
        {
            Eco_Array_Put(array, i, &default_value);
        }
    }

    return array;
}

void Eco_Array_Mark(struct Eco_GC_State* state, struct Eco_Array* array)
{
    unsigned int  index;

    for (index = 0; index < Eco_Array_Size(array); index++)
    {
        Eco_GC_State_MarkAny(state, &array->values[index]);
    }
    Eco_Object_Mark(state, &array->_);
}

struct Eco_Array* Eco_Array_Clone(struct Eco_CloneState* state,
                                  struct Eco_Array* array,
                                  bool forced)
{
    /*
     * TODO: This function just creates a plain copy of the array.
     *       For a better cloning mechanism we also need to clone
     *       the elements inside of it.
     */
    struct Eco_Array*  the_clone;
    unsigned int        index;

    if (!forced) return array;

    the_clone = Eco_Array_New(Eco_Array_Size(array));
    
    Eco_CloneState_RegisterClone(state,
                                 (struct Eco_Object*) array,
                                 (struct Eco_Object*) the_clone);

    for (index = 0; index < Eco_Array_Size(array); index++)
    {
        Eco_Any_AssignAny(&the_clone->values[index],
                          &array->values[index]);
    }

    return the_clone;
}

void Eco_Array_Del(struct Eco_Array* array)
{
    Eco_Object_Del(&array->_);
}


/*
 *    E l e m e n t   A c c e s s
 */

struct Eco_String* Eco_Array_ToString(struct Eco_Array* array)
{
    const unsigned int buffer_length = Eco_Array_Size(array) * 4 + 1;
    char               buffer[buffer_length];  // TODO: This might be a vulnerability
    unsigned int       buffer_index;
    unsigned int       array_index;

    buffer_index = 0;

    for (array_index = 0;
         array_index < Eco_Array_Size(array);
         array_index++)
    {
        if (Eco_Any_IsCharacter(*Eco_Array_At(array, array_index))) {
            buffer_index += Eco_Utf8_Encode(Eco_Any_AsCharacter(*Eco_Array_At(array, array_index)), &buffer[buffer_index]);
        }
    }

    buffer[buffer_index] = '\0';

    return Eco_String_New(buffer);
}
