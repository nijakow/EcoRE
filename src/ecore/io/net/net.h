#ifndef ECO_IO_NET_NET_H
#define ECO_IO_NET_NET_H


struct Eco_Net_Scheduler;
struct Eco_Net_Connection;

typedef void (*Eco_Net_Connection_ReadCallback)(struct Eco_Net_Connection*, struct Eco_Net_Scheduler*);
typedef void (*Eco_Net_Connection_Destructor)(struct Eco_Net_Connection*);

struct Eco_Net_Connection
{
    struct Eco_Net_Scheduler*        scheduler;
    unsigned int                     fd;
    Eco_Net_Connection_ReadCallback  read_callback;
    Eco_Net_Connection_Destructor    del;
};

struct Eco_Net_Connection* Eco_Net_Connection_New(int, Eco_Net_Connection_ReadCallback, Eco_Net_Connection_Destructor, unsigned int);
void Eco_Net_Connection_Delete(struct Eco_Net_Connection*);

#endif
