#include <sys/epoll.h>
#include <unistd.h>

#include "sched.h"

#include "net.h"

#include "../../vm/memory/memory.h"


struct Eco_Net_Sched
{
    int                 epoll_fd;
    unsigned int        epoll_events_max;
    struct epoll_event  epoll_events[];
};


struct Eco_Net_Sched* Eco_Net_Sched_New(unsigned int events)
{
    struct Eco_Net_Sched*  sched;

    sched                   = Eco_Memory_Alloc(sizeof(struct Eco_Net_Sched) + sizeof(struct epoll_event) * events);
    sched->epoll_fd         = epoll_create1(0); /* TODO: Error Handling */
    sched->epoll_events_max = events;

    return sched;
}

void Eco_Net_Sched_Delete(struct Eco_Net_Sched* sched)
{
    /* TODO: What happens to all the connections? */
    close(sched->epoll_fd);
    Eco_Memory_Free(sched);
}


void Eco_Net_Sched_Tick(struct Eco_Net_Sched* sched, int timeout)
{
    struct Eco_Net_Connection*  connection;
    unsigned int                event_index;
    unsigned int                event_count;
    unsigned int                bytes_read;
    char                        buffer[1024];

    event_count = epoll_wait(sched->epoll_fd,
                             sched->epoll_events,
                             sched->epoll_events_max,
                             timeout);
    
    for (event_index = 0; event_index < event_count; event_index++)
    {
        connection = (struct Eco_Net_Connection*) sched->epoll_events[event_index].data.ptr;
        bytes_read = read(connection->fd, buffer, sizeof(buffer));  /* TODO: Catch errors */
        if (connection->input_callback != NULL) {
            connection->input_callback(connection, buffer, bytes_read);
        }
    }
}
