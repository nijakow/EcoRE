#ifndef ECO_VM_SCHEDULER_H
#define ECO_VM_SCHEDULER_H

#include <ecore/vm/fiber/sched.h>


struct Eco_Scheduler
{
    struct {
        struct Eco_FiberQueue  running;
        struct Eco_FiberQueue  paused;
    } fiber_queues;
};

void Eco_Scheduler_Create(struct Eco_Scheduler*);
void Eco_Scheduler_Destroy(struct Eco_Scheduler*);

void Eco_Scheduler_Run(struct Eco_Scheduler*);

#endif
