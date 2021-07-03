#include "callback.h"

#include "../../objects/key.h"


void Eco_EConnect_InstallCallback(char* key_name, Eco_EConnect_Callback callback)
{
    struct Eco_Key*  key;

    key = Eco_Key_Find(key_name);

    if (key != NULL) {
        key->econnect_callback = callback;
        /* TODO, FIXME, XXX: Make the key GC-permanent */
    }
}
