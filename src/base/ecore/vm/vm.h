#ifndef ECO_VM_VM_H
#define ECO_VM_VM_H

#include <ecore/vm/scheduler.h>
#include <ecore/vm/memory/gc/gc_state.h>
#include <ecore/vm/memory/weak.h>


struct Eco_GC_State;
struct Eco_Code;

typedef void (*Eco_PeriodicFunc)();


struct Eco_VM
{
    struct Eco_Scheduler          scheduler;

    struct Eco_WeakObjectManager  weak_objects;
    struct Eco_GC_State           gc_state;

    struct Eco_Fiber*             fibers;
    Eco_Integer                   fiber_id_counter;

    struct {
        Eco_Any  lobby;
        
        Eco_Any  ctrue;
        Eco_Any  cfalse;

        Eco_Any  basic_error;
    } constants;

    struct timeval                start_time;

    Eco_PeriodicFunc              periodic_func;
};


void Eco_VM_Create(struct Eco_VM*);
void Eco_VM_Destroy(struct Eco_VM*);
void Eco_VM_Mark(struct Eco_GC_State*, struct Eco_VM*);
void Eco_VM_FreeAll(struct Eco_VM*);

static inline struct Eco_Scheduler* Eco_VM_GetScheduler(struct Eco_VM* vm) {
    return &vm->scheduler;
}

void Eco_VM_Run(struct Eco_VM*);

struct Eco_Fiber* Eco_VM_SpawnCode(struct Eco_VM*, struct Eco_Code*, Eco_Any*, unsigned int);
struct Eco_Fiber* Eco_VM_SpawnThunk(struct Eco_VM*, struct Eco_Code*);

bool Eco_VM_LoadImageFromFile(struct Eco_VM*, const char*);
bool Eco_VM_LoadImage(struct Eco_VM*, char*, unsigned long);

Eco_Integer Eco_VM_NewFiberId(struct Eco_VM*);

struct Eco_Fiber* Eco_VM_GetFiberById(struct Eco_VM*, Eco_Integer);

static inline void Eco_VM_SetPeriodicFunc(struct Eco_VM* vm, Eco_PeriodicFunc func) {
    vm->periodic_func = func;
}

#endif
