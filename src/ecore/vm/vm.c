#include "vm.h"

#include "fiber.h"
#include "core/interpreter.h"
#include "../io/econnect/file/file.h"
#include "../io/logging/log.h"


struct Eco_VM Eco_THE_VM;



void Eco_VM_Create(struct Eco_VM* vm)
{
    vm->fiber_queues.running = NULL;
    vm->fiber_queues.paused  = NULL;

    vm->net_scheduler = Eco_Net_Scheduler_New(4096);
}

void Eco_VM_Destroy(struct Eco_VM* vm)
{
    while (vm->fiber_queues.running != NULL) Eco_Fiber_Delete(vm->fiber_queues.running);
    while (vm->fiber_queues.paused != NULL)  Eco_Fiber_Delete(vm->fiber_queues.paused);

    Eco_Net_Scheduler_Delete(vm->net_scheduler);
}


static void Eco_VM_MarkQueue(struct Eco_GC_State* state, struct Eco_Fiber* queue)
{
    while (queue != NULL)
    {
        Eco_Fiber_Mark(state, queue);
        queue = queue->queue_next;
    }
}

void Eco_VM_Mark(struct Eco_GC_State* state, struct Eco_VM* vm)
{
    Eco_VM_MarkQueue(state, vm->fiber_queues.running);
    Eco_VM_MarkQueue(state, vm->fiber_queues.paused);
}

void Eco_VM_HandleEvents(struct Eco_VM* vm)
{
    if (vm->net_scheduler != NULL) {
        Eco_Net_Scheduler_Tick(vm->net_scheduler, vm->fiber_queues.running == NULL ? 1000 : 0);
    }
}



static inline void Eco_VM_AdvanceQueue(struct Eco_Fiber** queue)
{
    if (*queue != NULL) {
        *queue = (*queue)->queue_next;
    }
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


bool Eco_VM_LoadImage(struct Eco_VM* vm, const char* file)
{
    struct Eco_EConnect_Result  result;

    Eco_Log(Eco_Loglevel_INFO, "Loading file '%s'...\n", file);

    if (Eco_EConnect_ReadFile(file, &result)) {
        Eco_Log(Eco_Loglevel_INFO, "Loading file '%s' was successful!\n", file);
        // TODO: Start fiber
        Eco_EConnect_Result_Destroy(&result);
        return true;
    } else {
        Eco_Log(Eco_Loglevel_ERROR, "Can't load file '%s'!\n", file);
        return false;
    }
}
