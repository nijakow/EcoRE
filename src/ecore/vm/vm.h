#ifndef ECO_VM_VM_H
#define ECO_VM_VM_H

#include <ecore/vm/scheduler.h>
#include <ecore/vm/memory/gc/gc_state.h>


struct Eco_GC_State;
struct Eco_Code;


struct Eco_VM
{
    struct Eco_Scheduler  scheduler;
    struct Eco_GC_State   gc_state;

    struct Eco_Fiber*     fibers;

    struct {
        Eco_Any  lobby;
        
        Eco_Any  ctrue;
        Eco_Any  cfalse;
    }                     constants;
};

extern struct Eco_VM Eco_THE_VM;


void Eco_VM_Create(struct Eco_VM*);
void Eco_VM_Destroy(struct Eco_VM*);
void Eco_VM_Mark(struct Eco_GC_State*, struct Eco_VM*);
void Eco_VM_FreeAll(struct Eco_VM*);

void Eco_VM_Run(struct Eco_VM*);

struct Eco_Fiber* Eco_VM_SpawnThunk(struct Eco_VM*, struct Eco_Code*);
bool Eco_VM_LoadImageFromFile(struct Eco_VM*, const char*);

#endif
