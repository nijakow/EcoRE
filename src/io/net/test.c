#include <stdio.h>

#include "test.h"

#include "net.h"
#include "scheduler.h"
#include "socket.h"
#include "../../vm/vm.h"


void Eco_Net_Test_ClientCallback(struct Eco_Net_Connection* connection, struct Eco_Net_Scheduler* scheduler)
{
    int   received;
    char  buffer[1024];

    received = Eco_Net_Socket_Read(connection->fd, buffer, sizeof(buffer) - 1);

    if (received > 0) {
        buffer[received] = '\0';
        printf("Received: %s\n", buffer);
    }
}


void Eco_Net_Test_ServerCallback(struct Eco_Net_Connection* connection, struct Eco_Net_Scheduler* scheduler)
{
    struct Eco_Net_Connection*  client_connection;

    client_connection = Eco_Net_Connection_New(Eco_Net_Socket_AcceptClientV4(connection->fd), Eco_Net_Test_ClientCallback);

    Eco_Net_Scheduler_Register(scheduler, client_connection);

    printf("Connection registered!\n");
}

void Eco_Net_TestServer(int port)
{
    struct Eco_Net_Connection*  connection;

    connection = Eco_Net_Connection_New(Eco_Net_Socket_OpenServerV4(8888, 4), Eco_Net_Test_ServerCallback);

    Eco_Net_Scheduler_Register(Eco_THE_VM.net_scheduler, connection);
}
