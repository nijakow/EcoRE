#include <string.h>

#include "key.h"

#include "../../base/type.h"


static struct Eco_Key* Eco_KEYS = NULL;


struct Eco_Key* Eco_Key_New(const char* name)
{
    unsigned int     namelen;
    struct Eco_Key*  key;

    namelen                = strlen(name);
    key                    = Eco_Object_New_Derived(Eco_Type_KEY_TYPE, sizeof(struct Eco_Key) + (namelen + 1) * sizeof(char), 0);
    key->econnect_callback = NULL;

    memcpy(&(key->name), name, namelen + 1);

    key->next              = Eco_KEYS;
    key->prev              = NULL;
    Eco_KEYS               = key;

    return key;
}

struct Eco_Key* Eco_Key_Find(const char* name)
{
    struct Eco_Key*  key;

    for (key = Eco_KEYS; key != NULL; key = key->next)
    {
        if (strcmp(name, key->name) == 0) {
            return key;
        }
    }

    return Eco_Key_New(name);
}

void Eco_Key_Mark(struct Eco_GC_State* state, struct Eco_Key* key)
{
    Eco_Object_Mark(state, &(key->_));
}

void Eco_Key_Del(struct Eco_Key*  key)
{
    if (Eco_KEYS == key) {
        if (key->prev != NULL) Eco_KEYS = key->prev;
        else                   Eco_KEYS = key->next;
    }

    if (key->prev != NULL) key->prev->next = key->next;
    if (key->next != NULL) key->next->prev = key->prev;

    Eco_Object_Del(&(key->_));
}



/*
 *    T y p e C o r e
 */

struct Eco_TypeCore Eco_Key_TYPECORE;

void Eco_Key_Init()
{
    Eco_TypeCore_Create(&Eco_Key_TYPECORE, "Eco_Key");

    Eco_Key_TYPECORE.send = (Eco_TypeCore_SendFunc) Eco_Object_Send;
    Eco_Key_TYPECORE.mark = (Eco_TypeCore_MarkFunc) Eco_Key_Mark;
    Eco_Key_TYPECORE.del  = (Eco_TypeCore_DelFunc) Eco_Key_Del;
}

void Eco_Key_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_Key_TYPECORE);
}
