#pragma once

#include "fiber.h"
#include "../io/net/scheduler.h"

struct Eco_GC_State;


struct Eco_VM
{
    struct {
        struct Eco_Fiber*  running;
        struct Eco_Fiber*  paused;
    } fiber_queues;

    struct Eco_Net_Scheduler*  net_scheduler;
};

extern struct Eco_VM Eco_THE_VM;


void Eco_VM_Create(struct Eco_VM*);
void Eco_VM_Destroy(struct Eco_VM*);
void Eco_VM_Mark(struct Eco_GC_State*, struct Eco_VM*);
void Eco_VM_Run(struct Eco_VM*);
