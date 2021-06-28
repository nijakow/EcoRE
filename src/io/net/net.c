#include <unistd.h>

#include "net.h"

#include "../../vm/memory/memory.h"


struct Eco_Net_Connection* Eco_Net_Connection_New(int fd, Eco_Net_ReadCallback callback)
{
    struct Eco_Net_Connection*  connection;

    connection = Eco_Memory_Alloc(sizeof(struct Eco_Net_Connection));

    connection->fd            = fd;
    connection->read_callback = callback;

    return connection;
}

void Eco_Net_Connection_Delete(struct Eco_Net_Connection* connection)
{
    close(connection->fd);
    Eco_Memory_Free(connection);
}
