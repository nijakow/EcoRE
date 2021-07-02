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


void Eco_EConnect_Message_Create_ForReading(struct Eco_EConnect_Message* message,
                                            struct Eco_EConnect_State* state,
                                            char* buffer,
                                            unsigned int bufsize,
                                            void (*del)(u8*))
{
    message->state = state;
    Eco_IO_ByteStream_Create(&(message->bytes), buffer, bufsize, del);
}

void Eco_EConnect_Message_Destroy(struct Eco_EConnect_Message* message)
{
    Eco_IO_ByteStream_Destroy(&(message->bytes));
}


bool Eco_EConnect_Message_Parse(struct Eco_EConnect_Message* message,
                                struct Eco_EConnect_ParseResult* result)
{
    struct Eco_Key*  msg_key;

    /* TODO: This unchecked cast is dangerous! */
    msg_key = (struct Eco_Key*) Eco_EConnect_ParseObject_ByID(message);

    if (msg_key->econnect_func != NULL) {
        return msg_key->econnect_func(message);
    } else {
        /* TODO: Error */
        return NULL;
    }
}
