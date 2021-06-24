#pragma once

struct Eco_Net_Scheduler;
struct Eco_Net_Connection;


typedef void (*Eco_Net_ReadCallback)(struct Eco_Net_Connection*, struct Eco_Net_Scheduler*);

struct Eco_Net_Connection
{
    unsigned int          fd;
    Eco_Net_ReadCallback  read_callback;
};
