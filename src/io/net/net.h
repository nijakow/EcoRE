#pragma once

struct Eco_Net_Scheduler;
struct Eco_Net_Connection;


typedef void (*Eco_Net_ReadCallback)(struct Eco_Net_Connection*, struct Eco_Net_Scheduler*);

struct Eco_Net_Connection
{
    struct Eco_Net_Scheduler*  scheduler;
    unsigned int               fd;
    Eco_Net_ReadCallback       read_callback;
};

struct Eco_Net_Connection* Eco_Net_Connection_New(int, Eco_Net_ReadCallback);
void Eco_Net_Connection_Delete(struct Eco_Net_Connection*);
