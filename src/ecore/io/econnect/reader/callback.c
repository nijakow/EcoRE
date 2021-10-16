#include "callback.h"

#include <ecore/objects/misc/key/key.h>
#include <ecore/vm/memory/gc/gc.h>


void Eco_EConnect_InstallCallback(char* key_name, Eco_EConnect_Callback callback)
{
    struct Eco_Key*  key;

    key = Eco_Key_Find(key_name);

    if (key != NULL) {
        key->econnect_callback = callback;
        Eco_GC_MakeSticky((struct Eco_Object*) key);
    }
}
