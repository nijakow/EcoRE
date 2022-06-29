#ifndef ECO_VM_FIBER_STACKOPS_H
#define ECO_VM_FIBER_STACKOPS_H

#include <ecore/base/defs.h>
#include <ecore/base/any.h>

#include "fiber.h"


struct Eco_Frame* Eco_Fiber_PushFrame(struct Eco_Fiber*, Eco_Any, unsigned int, unsigned int, unsigned int);
void Eco_Fiber_PopFrame(struct Eco_Fiber*);

void Eco_Fiber_ResetFrame(struct Eco_Fiber*);


static inline struct Eco_Frame* Eco_Fiber_Top(struct Eco_Fiber* fiber)
{
    return fiber->top;
}

static inline bool Eco_Fiber_HasTop(struct Eco_Fiber* fiber)
{
    return Eco_Fiber_Top(fiber) != NULL;
}

static inline bool Eco_Fiber_Push(struct Eco_Fiber* fiber, Eco_Any* src)
{
    Eco_Any_AssignAny((Eco_Any*) fiber->stack_pointer, src);
    fiber->stack_pointer += sizeof(Eco_Any);
    return true;
}

static inline bool Eco_Fiber_Pop(struct Eco_Fiber* fiber, Eco_Any* dest)
{
    fiber->stack_pointer -= sizeof(Eco_Any);
    Eco_Any_AssignAny(dest, (Eco_Any*) fiber->stack_pointer);
    return true;
}

static inline bool Eco_Fiber_Drop(struct Eco_Fiber* fiber)
{
    fiber->stack_pointer -= sizeof(Eco_Any);
    return true;
}

static inline bool Eco_Fiber_Dup(struct Eco_Fiber* fiber)
{
    Eco_Any_AssignAny((Eco_Any*) fiber->stack_pointer, (Eco_Any*) (fiber->stack_pointer - sizeof(Eco_Any)));
    fiber->stack_pointer += sizeof(Eco_Any);
    return true;
}

static inline Eco_Any* Eco_Fiber_Nth(struct Eco_Fiber* fiber, unsigned int n)
{
    return (Eco_Any*) (fiber->stack_pointer - n * sizeof(Eco_Any));
}

static inline Eco_Any* Eco_Fiber_Peek(struct Eco_Fiber* fiber)
{
    return Eco_Fiber_Nth(fiber, 1);
}

static inline void* Eco_Fiber_StackAlloc(struct Eco_Fiber* fiber, unsigned int size)
{
    void* ptr;

    ptr                  = fiber->stack_pointer;
    fiber->stack_pointer = fiber->stack_pointer + size;

    return ptr;
}

#endif
