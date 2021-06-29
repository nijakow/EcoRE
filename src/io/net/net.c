#include <unistd.h>

#include "net.h"

#include "scheduler.h"
#include "../../vm/memory/memory.h"


struct Eco_Net_Connection* Eco_Net_Connection_New(int fd,
                                                  Eco_Net_Connection_ReadCallback read_cb,
                                                  Eco_Net_Connection_Destructor del,
                                                  unsigned int size)
{
    struct Eco_Net_Connection*  connection;

    connection = Eco_Memory_Alloc(size);

    connection->scheduler     = NULL;
    connection->fd            = fd;
    connection->read_callback = read_cb;
    connection->del           = del;

    return connection;
}

void Eco_Net_Connection_Delete(struct Eco_Net_Connection* connection)
{
    if (connection->scheduler != NULL) {
        Eco_Net_Scheduler_Unregister(connection->scheduler, connection);
    }
    if (connection->del != NULL) {
        connection->del(connection);
    }
    close(connection->fd);
    Eco_Memory_Free(connection);
}
