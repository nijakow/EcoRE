#ifndef ECO_IO_NET_SCHEDULER_H
#define ECO_IO_NET_SCHEDULER_H


struct Eco_Net_Scheduler
{
    struct Eco_Net_Connection* connections;
};

void Eco_Net_Scheduler_Create(struct Eco_Net_Scheduler*);
void Eco_Net_Scheduler_Destroy(struct Eco_Net_Scheduler*);
void Eco_Net_Scheduler_Tick(struct Eco_Net_Scheduler*, int);
void Eco_Net_Scheduler_Register(struct Eco_Net_Scheduler*, struct Eco_Net_Connection*);
void Eco_Net_Scheduler_Unregister(struct Eco_Net_Scheduler*, struct Eco_Net_Connection*);

#endif
