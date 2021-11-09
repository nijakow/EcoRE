#include <string.h>

#include "string.h"

#include <ecore/objects/base/type.h>


/*
 *    T y p e C o r e
 */

static struct Eco_TypeCore Eco_String_TYPECORE;
       struct Eco_Type*    Eco_String_TYPE;


void Eco_String_Init()
{
    Eco_TypeCore_Create(&Eco_String_TYPECORE, "Eco_String");

    Eco_String_TYPECORE.send = (Eco_TypeCore_SendFunc) Eco_Object_Send;
    Eco_String_TYPECORE.mark = (Eco_TypeCore_MarkFunc) Eco_String_Mark;
    Eco_String_TYPECORE.del  = (Eco_TypeCore_DelFunc)  Eco_String_Del;

    Eco_String_TYPE          = Eco_Type_NewPrefab(&Eco_String_TYPECORE);
}

void Eco_String_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_String_TYPECORE);
}


/*
 *    B a s i c s
 */

struct Eco_String* Eco_String_New(const char* text)
{
    unsigned int        textlen;
    struct Eco_String*  string;

    textlen                 = strlen(text);
    string                  = Eco_Object_New(Eco_String_TYPE, sizeof(struct Eco_String) + textlen * sizeof(char), 0);
    string->byte_count      = textlen;
    string->character_count = textlen;  // TODO: Count UTF-8 code points!

    memcpy(&string->bytes, text, textlen);

    return string;
}

void Eco_String_Mark(struct Eco_GC_State* state, struct Eco_String* string)
{
    Eco_Object_Mark(state, &string->_);
}

void Eco_String_Del(struct Eco_String* string)
{
    Eco_Object_Del(&string->_);
}
