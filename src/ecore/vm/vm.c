#include "vm.h"

#include "builtins/builtins.h"
#include "core/interpreter.h"

#include <ecore/vm/fiber/fiber.h>
#include <ecore/vm/fiber/gc.h>
#include <ecore/io/econnect/file/file.h>
#include <ecore/io/logging/log.h>
#include <ecore/vm/memory/gc/gc.h>
#include <ecore/vm/memory/gc/gc_state.h>


/*
 *
 *    C o n s t r u c t o r s   a n d   D e s t r u c t o r s
 *
 */

void Eco_VM_Create(struct Eco_VM* vm)
{
    vm->fibers = NULL;
    
    vm->constants.lobby  = Eco_Any_Default();
    vm->constants.ctrue  = Eco_Any_Default();
    vm->constants.cfalse = Eco_Any_Default();

    Eco_GC_State_Create(&vm->gc_state, vm);
    Eco_Scheduler_Create(&vm->scheduler);
}

void Eco_VM_Destroy(struct Eco_VM* vm)
{
    while (vm->fibers != NULL)
        Eco_Fiber_Delete(vm->fibers);
    Eco_Scheduler_Destroy(&vm->scheduler);
    Eco_GC_State_Destroy(&vm->gc_state);
}

void Eco_VM_Mark(struct Eco_GC_State* state, struct Eco_VM* vm)
{
    struct Eco_Fiber*  fiber;

    for (fiber = vm->fibers; fiber != NULL; fiber = fiber->next)
        Eco_Fiber_Mark(state, fiber);
    
    Eco_GC_State_MarkAny(state, vm->constants.lobby);
    Eco_GC_State_MarkAny(state, vm->constants.ctrue);
    Eco_GC_State_MarkAny(state, vm->constants.cfalse);
}

void Eco_VM_FreeAll(struct Eco_VM* vm)
{
    Eco_GC_FreeAll(&vm->gc_state);
}


/*
 *
 *    M a i n   O p e r a t i o n   F u n c t i o n s
 *
 */

void Eco_VM_HandleEvents(struct Eco_VM* vm)
{
}

void Eco_VM_Run(struct Eco_VM* vm)
{
    /*
     * This is a pretty ugly while condition, but it enables us to terminate
     * after the last fiber has exited. Of course: TODO!
     *                                                    - nijakow
     */
    while (vm->scheduler.fiber_queues.running.fibers != NULL || vm->scheduler.fiber_queues.paused.fibers != NULL)
    {
        Eco_Scheduler_Run(&vm->scheduler);
        Eco_VM_HandleEvents(vm);
        Eco_GC_Step(&vm->gc_state); // TODO: Only call this if necessary!
    }
}


/*
 *
 *    B o o t s t r a p p i n g
 *
 */

struct Eco_Fiber* Eco_VM_SpawnThunk(struct Eco_VM* vm, struct Eco_Code* code)
{
    struct Eco_Fiber*  fiber;

    fiber = Eco_Fiber_New(vm, 1024 * 1024 * 4 /* 4MB of stack */);

    Eco_Fiber_EnterThunk(fiber, &vm->constants.lobby, code);
    Eco_Fiber_SetRunning(fiber);

    return fiber;
}

bool Eco_VM_LoadImage(struct Eco_VM* vm, char* image, unsigned long image_size)
{
    struct Eco_EConnect_Result  result;
    struct Eco_Code*            code;

    if (Eco_EConnect_LoadImage(&result, image, image_size)) {
        if (Eco_EConnect_Result_ExpectObject(&result, (struct Eco_Object**) &code)) {
            Eco_VM_SpawnThunk(vm, code);
        } else {
            Eco_Log_Error("Expected code, got something different (while loading image)!\n");
        }
        Eco_EConnect_Result_Destroy(&result);
        return true;
    } else {
        Eco_Log_Error("Can't load image!\n");
        return false;
    }   
}

bool Eco_VM_LoadImageFromFile(struct Eco_VM* vm, const char* file)
{
    struct Eco_EConnect_Result  result;
    struct Eco_Code*            code;

    if (Eco_EConnect_ReadFile(file, &result)) {
        if (Eco_EConnect_Result_ExpectObject(&result, (struct Eco_Object**) &code)) {
            Eco_VM_SpawnThunk(vm, code);
        } else {
            Eco_Log_Error("Expected code, got something different (while loading file '%s')!\n", file);
        }
        Eco_EConnect_Result_Destroy(&result);
        return true;
    } else {
        Eco_Log_Error("Can't load file '%s'!\n", file);
        return false;
    }
}
