#ifndef ECO_VM_FIBER_FIBER_H
#define ECO_VM_FIBER_FIBER_H

#include "states.h"


struct Eco_Frame;
struct Eco_Message;
struct Eco_Code;
struct Eco_Environment;
struct Eco_VM;
struct Eco_GC_State;


struct Eco_Fiber
{
    enum Eco_Fiber_State  state;

    struct Eco_VM*          vm;
    struct Eco_Fiber**      prev;
    struct Eco_Fiber*       next;

    struct Eco_Scheduler*   scheduler;
    struct Eco_FiberQueue*  queue;
    struct Eco_Fiber**      queue_prev;
    struct Eco_Fiber*       queue_next;

    struct Eco_Frame*       top;
    char*                   stack_pointer;
    char*                   stack_max;
    char                    stack[];
};

struct Eco_Fiber* Eco_Fiber_New(struct Eco_VM*, unsigned int);
void Eco_Fiber_Delete(struct Eco_Fiber*);
void Eco_Fiber_Mark(struct Eco_GC_State*, struct Eco_Fiber*);

#endif
