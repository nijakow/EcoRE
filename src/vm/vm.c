#include "vm.h"

#include "fiber.h"
#include "interpreter.h"


void Eco_VM_Create(struct Eco_VM* vm)
{
    vm->fiber_queues.running = NULL;
    vm->fiber_queues.paused  = NULL;
}

void Eco_VM_Destroy(struct Eco_VM* vm)
{
    while (vm->fiber_queues.running != NULL) Eco_Fiber_Delete(vm->fiber_queues.running);
    while (vm->fiber_queues.paused != NULL)  Eco_Fiber_Delete(vm->fiber_queues.paused);
}

void Eco_VM_Mark(struct Eco_GC_State* state, struct Eco_VM* vm)
{
    /* TODO */
}

void Eco_VM_HandleEvents(struct Eco_VM* vm)
{
    /* TODO */
}



static inline void Eco_VM_AdvanceQueue(struct Eco_Fiber** queue)
{
    *queue = (*queue)->queue_next;
}

void Eco_VM_Run(struct Eco_VM* vm)
{
    struct Eco_Fiber*  fiber;

    while (true)
    {
        if (vm->fiber_queues.running != NULL) {
            fiber = vm->fiber_queues.running;

            Eco_Fiber_Run(fiber);

            if (fiber->state == Eco_Fiber_State_RUNNING) {
                Eco_VM_AdvanceQueue(&(vm->fiber_queues.running));
            } else if (Eco_Fiber_State_IsError(fiber->state)) {
                /* TODO: Catch error */
                Eco_Fiber_Delete(fiber);
            } else if (fiber->state == Eco_Fiber_State_TERMINATED) {
                Eco_Fiber_Delete(fiber);
            }
        }
        Eco_VM_HandleEvents(vm);
    }
}
