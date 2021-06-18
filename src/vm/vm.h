#pragma once

#include "fiber.h"


struct Eco_VM
{
    struct Eco_Fiber*  fibers;
    
    struct Eco_Fiber*  running_queue;
};


void Eco_VM_Create(struct Eco_VM*);
void Eco_VM_Destroy(struct Eco_VM*);
void Eco_VM_Run(struct Eco_VM*);

