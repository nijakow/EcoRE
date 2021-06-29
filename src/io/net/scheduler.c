#include <sys/epoll.h>
#include <unistd.h>

#include "scheduler.h"

#include "net.h"

#include "../../vm/memory/memory.h"


struct Eco_Net_Scheduler
{
    int                 epoll_fd;
    unsigned int        epoll_events_max;
    struct epoll_event  epoll_events[];
};


struct Eco_Net_Scheduler* Eco_Net_Scheduler_New(unsigned int events)
{
    struct Eco_Net_Scheduler*  sched;

    sched                   = Eco_Memory_Alloc(sizeof(struct Eco_Net_Scheduler) + sizeof(struct epoll_event) * events);
    sched->epoll_fd         = epoll_create1(0); /* TODO: Error Handling */
    sched->epoll_events_max = events;

    return sched;
}

void Eco_Net_Scheduler_Delete(struct Eco_Net_Scheduler* sched)
{
    if (sched != NULL) {
        /* TODO: What happens to all the connections? */
        close(sched->epoll_fd);
        Eco_Memory_Free(sched);
    }
}

void Eco_Net_Scheduler_Register(struct Eco_Net_Scheduler* sched, struct Eco_Net_Connection* conn)
{
    struct epoll_event  event;

    event.events    = EPOLLIN;
    event.data.ptr  = conn;

    epoll_ctl(sched->epoll_fd, EPOLL_CTL_ADD, conn->fd, &event);

    conn->scheduler = sched;
}

void Eco_Net_Scheduler_Unregister(struct Eco_Net_Scheduler* sched, struct Eco_Net_Connection* conn)
{
    epoll_ctl(sched->epoll_fd, EPOLL_CTL_DEL, conn->fd, NULL);

    conn->scheduler = NULL;
}

void Eco_Net_Scheduler_Tick(struct Eco_Net_Scheduler* sched, int timeout)
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
        if (connection->read_callback != NULL) {
            connection->read_callback(connection, sched);
        }
    }
}
