#include "econnect.h"

#include "parser.h"

#include "../../objects/key.h"
#include "../../vm/memory/memory.h"


void Eco_EConnect_State_Create(struct Eco_EConnect_State* state)
{
    state->objects_by_id = NULL;
    state->objects_by_id_max = 0;
}

void Eco_EConnect_State_Destroy(struct Eco_EConnect_State* state)
{
    if (state->objects_by_id != NULL) {
        Eco_Memory_Free(state->objects_by_id);
    }
}


void Eco_EConnect_Init()
{
}

void Eco_EConnect_Terminate()
{
}
