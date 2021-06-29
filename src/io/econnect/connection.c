#include <stddef.h>

#include "connection.h"

#include "../net/socket.h"
#include "../net/scheduler.h"


struct Eco_EConnect_Connection
{
    struct Eco_Net_Connection  _;
};


void Eco_EConnect_Connection_Callback(struct Eco_Net_Connection* connection,
                                      struct Eco_Net_Scheduler* scheduler)
{
}

void Eco_EConnect_Connection_Destructor(struct Eco_EConnect_Connection* connection)
{
    /* TODO */
}

struct Eco_EConnect_Connection* Eco_EConnect_Connection_New(int socket_fd)
{
    struct Eco_EConnect_Connection*  connection;

    connection = (struct Eco_EConnect_Connection*)
                    Eco_Net_Connection_New(socket_fd,
                                           (Eco_Net_Connection_ReadCallback) Eco_EConnect_Connection_Callback,
                                           (Eco_Net_Connection_Destructor) Eco_EConnect_Connection_Destructor,
                                           sizeof(struct Eco_EConnect_Connection));
    
    /* TODO: Initialize fields */

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
