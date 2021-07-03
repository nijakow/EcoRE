#include "instance.h"

#include "../../objects/key.h"
#include "../../vm/memory/memory.h"


void Eco_EConnect_Instance_Create(struct Eco_EConnect_Instance* state)
{
    state->objects_by_id = NULL;
    state->objects_by_id_max = 0;
}

void Eco_EConnect_Instance_Destroy(struct Eco_EConnect_Instance* state)
{
    if (state->objects_by_id != NULL) {
        Eco_Memory_Free(state->objects_by_id);
    }
}
