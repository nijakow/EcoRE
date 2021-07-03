#include <string.h>

#include "key.h"

#include "type.h"


static struct Eco_Key* Eco_KEYS = NULL;


struct Eco_Key* Eco_Key_New(const char* name)
{
    unsigned int     namelen;
    struct Eco_Key*  key;

    namelen                = strlen(name);
    key                    = Eco_Object_New(Eco_Type_KEY_TYPE, sizeof(struct Eco_Key) + namelen * sizeof(char));
    key->econnect_callback = NULL;

    memcpy(&(key->name), name, namelen + 1);

    key->next              = Eco_KEYS;
    key->prev              = NULL;
    Eco_KEYS               = key;
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
