#include "vm.h"

void Eco_VM_Create(struct Eco_VM* vm)
{
    vm->fibers = NULL;
    vm->running_queue = NULL;
}

void Eco_VM_Destroy(struct Eco_VM* vm)
{
    /* TODO */
}

