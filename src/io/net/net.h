#pragma once

struct Eco_Net_Connection;


typedef void (*Eco_Net_InputCallback)(struct Eco_Net_Connection*, char*, unsigned int);

struct Eco_Net_Connection
{
    unsigned int           fd;
    Eco_Net_InputCallback  input_callback;
};
