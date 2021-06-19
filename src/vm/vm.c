#include "vm.h"

#include "fiber.h"


void Eco_VM_Create(struct Eco_VM* vm)
{
    vm->fibers = NULL;
    vm->running_queue = NULL;
}

void Eco_VM_Destroy(struct Eco_VM* vm)
{
    /* TODO */
}


void Eco_VM_HandleEvents(struct Eco_VM* vm)
{
    /* TODO */
}

void Eco_VM_Run(struct Eco_VM* vm)
{
    struct Eco_Fiber*  fiber;

    while (true)
    {
        if (vm->running_queue != NULL) {
            fiber = vm->running_queue;

            Eco_Fiber_Run(fiber);

            if (fiber->state == Eco_Fiber_State_RUNNING) {
                vm->running_queue = vm->running_queue->queue_next;
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
