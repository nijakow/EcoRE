#pragma once

#include "fiber.h"

struct Eco_GC_State;


struct Eco_VM
{
    struct {
        struct Eco_Fiber*  running;
        struct Eco_Fiber*  paused;
    } fiber_queues;
};


void Eco_VM_Create(struct Eco_VM*);
void Eco_VM_Destroy(struct Eco_VM*);
void Eco_VM_Mark(struct Eco_GC_State*, struct Eco_VM*);
void Eco_VM_Run(struct Eco_VM*);
