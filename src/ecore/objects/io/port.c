#include <unistd.h>

#include "port.h"

#include <ecore/objects/base/type.h>
#include <ecore/vm/fiber/sched.h>
#include <ecore/util/utf8.h>


/*
 *    T y p e C o r e
 */

static struct Eco_TypeCore Eco_Port_TYPECORE;
       struct Eco_Type*    Eco_Port_TYPE;


void Eco_Port_Init()
{
    Eco_TypeCore_Create(&Eco_Port_TYPECORE, "Eco_Port");

    Eco_Port_TYPECORE.send = (Eco_TypeCore_SendFunc) Eco_Object_Send;
    Eco_Port_TYPECORE.mark = (Eco_TypeCore_MarkFunc) Eco_Port_Mark;
    Eco_Port_TYPECORE.del  = (Eco_TypeCore_DelFunc)  Eco_Port_Del;

    Eco_Port_TYPE          = Eco_Type_NewPrefab(&Eco_Port_TYPECORE);
}

void Eco_Port_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_Port_TYPECORE);
}


/*
 *    B a s i c s
 */

struct Eco_Port* Eco_Port_New(struct Eco_Scheduler* scheduler, unsigned int fd)
{
    struct Eco_Port*  port;

    port = Eco_Object_New(Eco_Port_TYPE, sizeof(struct Eco_Port));

    if (port != NULL) {
        port->scheduler              = scheduler;
        port->waiting_fiber          = NULL;
        port->next                   = NULL;
        port->fd                     = fd;
        port->input_buffer_read_head = 0;
        port->input_buffer_fill      = 0;
        port->output_buffer_fill     = 0;
    }

    return port;
}

void Eco_Port_Mark(struct Eco_GC_State* state, struct Eco_Port* port)
{
    Eco_Object_Mark(state, &port->_);
}

void Eco_Port_Del(struct Eco_Port* port)
{
    Eco_Object_Del(&port->_);
}

/*
 *    S c h e d u l i n g
 */

void Eco_Port_Reactivate(struct Eco_Port* port)
{
    if (port->waiting_fiber_func != NULL) {
        port->waiting_fiber_func(port->waiting_fiber, port);
        port->waiting_fiber_func = NULL;
    }
}


/*
 *    I / O
 */

void Eco_Port_Read(struct Eco_Port* port)
{
    ssize_t  bytes_read;

    bytes_read = read(port->fd, port->input_buffer, Eco_Port_INPUT_BUFFER_SIZE);

    if (bytes_read <= 0) {
        // TODO: Error
    } else {
        port->input_buffer_read_head = 0;
        port->input_buffer_fill      = bytes_read;
    }
}

bool Eco_Port_FlushOutput(struct Eco_Port* port)
{
    bool  result;

    result                   = write(port->fd, port->output_buffer, port->output_buffer_fill);
    port->output_buffer_fill = 0;

    return result;
}

bool Eco_Port_ReadByte(struct Eco_Port* port, char* c)
{
    if (port->input_buffer_read_head < port->input_buffer_fill) {
        *c = port->input_buffer[port->input_buffer_read_head];
        port->input_buffer_read_head++;
        return true;
    } else {
        return false;
    }
}

bool Eco_Port_WriteBytes(struct Eco_Port* port, char* c, unsigned int count)
{
    unsigned int  i;

    for (i = 0; i < count; i++)
    {
        if (port->output_buffer_fill == Eco_Port_OUTPUT_BUFFER_SIZE) {
            Eco_Port_FlushOutput(port);
            port->output_buffer_fill = 0;
        }
        port->output_buffer[port->output_buffer_fill++] = c[i];
    }

    return true;
}

bool Eco_Port_WriteChar(struct Eco_Port* port, Eco_Codepoint codepoint)
{
    char bytes[4];

    return Eco_Port_WriteBytes(port, bytes, Eco_Utf8_Encode(codepoint, bytes));
}
