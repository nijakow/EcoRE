#include <stddef.h>
#include <string.h>
#include <assert.h>

#include "connection.h"
#include "econnect.h"
#include "parser.h"

#include "../net/socket.h"
#include "../net/scheduler.h"

#include "../../vm/memory/memory.h"


struct Eco_EConnect_Connection
{
    struct Eco_Net_Connection  _;

    unsigned int               message_length;
    unsigned int               message_fill;
    char*                      message;
};



void Eco_EConnect_Connection_Feed(struct Eco_EConnect_Connection* connection,
                                  char* buffer,
                                  unsigned int byte_count)
{
    unsigned int                     i;
    struct Eco_EConnect_Reader       reader;
    struct Eco_EConnect_ParseResult  result;

    i = 0;
    while (i < byte_count)
    {
        if (connection->message == NULL) {
            connection->message_length = (connection->message_length << 7) | (buffer[i] & 0x7f);

            if ((buffer[i] & 0x80) == 0x00) {
                if (byte_count - i - 1 >= connection->message_length) {
                    Eco_EConnect_Reader_Create(&reader, NULL, &buffer[i + 1], connection->message_length, NULL);
                    Eco_EConnect_Parse(&reader, &result);
                    Eco_EConnect_Reader_Destroy(&reader);
                    
                    i += connection->message_length;
                    connection->message_length = 0;
                } else {
                    connection->message_fill = 0;
                    connection->message      = Eco_Memory_Alloc(connection->message_length * sizeof(char));
                }
            }
            
            i++;
        } else {
            if (byte_count - i <= connection->message_length - connection->message_fill) {
                memcpy(connection->message, &buffer[i], byte_count - i);
                connection->message_fill += byte_count - i;
                i = byte_count;
            } else {
                memcpy(connection->message, &buffer[i], connection->message_length - connection->message_fill);
                connection->message_fill = connection->message_length;
                i += connection->message_length - connection->message_fill;
            }

            assert(connection->message_fill <= connection->message_length);

            if (connection->message_fill == connection->message_length) {
                Eco_EConnect_Reader_Create(&reader, NULL, connection->message, connection->message_length, NULL);
                Eco_EConnect_Parse(&reader, &result);
                Eco_EConnect_Reader_Destroy(&reader);

                Eco_Memory_Free(connection->message);
                connection->message_length = 0;
                connection->message        = NULL;
            }
        }
    }
}



/*
 *
 *    T h i s   p a r t   w i l l   b e   r e w o r k e d   l a t e r ! ! !
 *
 */

void Eco_EConnect_Connection_Callback(struct Eco_Net_Connection* connection,
                                      struct Eco_Net_Scheduler* scheduler)
{
    unsigned int  bytes_read;
    char          buffer[1024];

    bytes_read = Eco_Net_Socket_Read(connection->fd, buffer, sizeof(buffer));

    if (bytes_read > 0) {
        Eco_EConnect_Connection_Feed((struct Eco_EConnect_Connection*) connection, buffer, bytes_read);
    } else {
        /* TODO */
    }
}

void Eco_EConnect_Connection_Destructor(struct Eco_EConnect_Connection* connection)
{
    if (connection->message != NULL) {
        Eco_Memory_Free(connection->message);
    }
}

struct Eco_EConnect_Connection* Eco_EConnect_Connection_New(int socket_fd)
{
    struct Eco_EConnect_Connection*  connection;

    connection = (struct Eco_EConnect_Connection*)
                    Eco_Net_Connection_New(socket_fd,
                                           (Eco_Net_Connection_ReadCallback) Eco_EConnect_Connection_Callback,
                                           (Eco_Net_Connection_Destructor) Eco_EConnect_Connection_Destructor,
                                           sizeof(struct Eco_EConnect_Connection));
    
    connection->message        = NULL;
    connection->message_length = 0;

    return connection;
}


void Eco_EConnect_Server_Callback(struct Eco_Net_Connection* connection,
                                  struct Eco_Net_Scheduler* scheduler)
{
    int                              client_socket_fd;
    struct Eco_EConnect_Connection*  client_connection;

    client_socket_fd  = Eco_Net_Socket_AcceptClientV4(connection->fd);
    client_connection =  Eco_EConnect_Connection_New(client_socket_fd);

    Eco_Net_Scheduler_Register(scheduler, (struct Eco_Net_Connection*) client_connection);
}


struct Eco_Net_Connection* Eco_EConnect_Server_New(int port, int listen_queue_length)
{
    int  server_socket_fd;

    server_socket_fd = Eco_Net_Socket_OpenServerV4(port, listen_queue_length);

    return Eco_Net_Connection_New(server_socket_fd, Eco_EConnect_Server_Callback, NULL, sizeof(struct Eco_Net_Connection));
}
