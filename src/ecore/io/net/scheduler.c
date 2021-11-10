#include <unistd.h>
#include <sys/select.h>

#include "scheduler.h"
#include "net.h"


void Eco_Net_Scheduler_Create(struct Eco_Net_Scheduler* sched)
{
    sched->connections = NULL;
}

void Eco_Net_Scheduler_Destroy(struct Eco_Net_Scheduler* sched)
{
    while (sched->connections)
        Eco_Net_Connection_Delete(sched->connections);
}

void Eco_Net_Scheduler_Register(struct Eco_Net_Scheduler* sched, struct Eco_Net_Connection* conn)
{
    conn->scheduler    =  sched;
    conn->prev         = &sched->connections;
    conn->next         =  sched->connections;
    if (sched->connections != NULL)
        sched->connections->prev = &conn->next;
    sched->connections =  conn;
}

void Eco_Net_Scheduler_Unregister(struct Eco_Net_Scheduler* sched, struct Eco_Net_Connection* conn)
{
    conn->scheduler  = NULL;
    *(conn->prev)    = conn->next;
    if (conn->next != NULL)
        conn->next->prev = conn->prev;
}

void Eco_Net_Scheduler_Tick(struct Eco_Net_Scheduler* sched, int timeout)
{
    struct Eco_Net_Connection*  connection;
    fd_set                      in_set;
    int                         max_fd;
    struct timeval              timeout_struct;

    max_fd = 0;
    FD_ZERO(&in_set);
    for (connection = sched->connections;
         connection != NULL;
         connection = connection->next)
    {
        if (connection->fd > max_fd)
            max_fd = connection->fd;
        if (connection->read_callback != NULL)
            FD_SET(connection->fd, &in_set);
    }

    timeout_struct.tv_sec  = 0;
    timeout_struct.tv_usec = timeout;

    select(max_fd + 1, &in_set, NULL, NULL, &timeout_struct);
    
    for (connection = sched->connections;
         connection != NULL;
         connection = connection->next)
    {
        if (FD_ISSET(connection->fd, &in_set)) {
            connection->read_callback(connection, sched);
        }
    }
}
