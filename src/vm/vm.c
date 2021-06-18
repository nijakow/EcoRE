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
    struct Eco_Fiber*  next_fiber;

    while (true)
    {
        if (vm->running_queue != NULL) {
            Eco_Fiber_Run(vm->running_queue);
            vm->running_queue = vm->running_queue->next;
        }
        Eco_VM_HandleEvents(vm);
    }
}

