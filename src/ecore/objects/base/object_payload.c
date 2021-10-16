#include <ecore/eco.h>

#include "object_payload.h"

#include <ecore/vm/memory/memory.h>


struct Eco_Object_Payload* Eco_Object_Payload_New(unsigned int size)
{
    struct Eco_Object_Payload*  payload;

    if (size == 0) return NULL;

    payload       = Eco_Memory_Alloc(sizeof(struct Eco_Object_Payload) + size);
    payload->size = size;

    return payload;
}

void Eco_Object_Payload_Delete(struct Eco_Object_Payload* payload)
{
    Eco_Memory_Free(payload);
}

struct Eco_Object_Payload* Eco_Object_Payload_Resize(struct Eco_Object_Payload* payload, unsigned int new_size)
{
    if (payload == NULL) {
        return Eco_Object_Payload_New(new_size);
    } else if (new_size == 0) {
        Eco_Object_Payload_Delete(payload);
        return NULL;
    } else {
        return Eco_Memory_Realloc(payload, sizeof(struct Eco_Object_Payload) + new_size);
    }
}
