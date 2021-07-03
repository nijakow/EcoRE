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


void Eco_EConnect_Reader_Create(struct Eco_EConnect_Reader* reader,
                                struct Eco_EConnect_State* state,
                                char* buffer,
                                unsigned int bufsize,
                                void (*del)(u8*))
{
    reader->state = state;
    Eco_IO_ByteInputStream_Create(&(reader->bytes), buffer, bufsize, del);
}

void Eco_EConnect_Reader_Destroy(struct Eco_EConnect_Reader* reader)
{
    Eco_IO_ByteInputStream_Destroy(&(reader->bytes));
}


bool Eco_EConnect_Reader_Parse(struct Eco_EConnect_Reader* reader,
                               struct Eco_EConnect_ParseResult* result)
{
    struct Eco_Key*  msg_key;

    /* TODO: This unchecked cast is dangerous! */
    msg_key = (struct Eco_Key*) Eco_EConnect_ParseObject_ByID(reader);

    if (msg_key->econnect_func != NULL) {
        return msg_key->econnect_func(reader, result);
    } else {
        /* TODO: Error */
        return NULL;
    }
}
