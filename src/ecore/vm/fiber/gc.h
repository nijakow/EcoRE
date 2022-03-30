#ifndef ECO_VM_FIBER_GC_H
#define ECO_VM_FIBER_GC_H

#include "fiber.h"

#include <ecore/vm/memory/gc/gc.h>

void Eco_Fiber_Mark(struct Eco_GC_State*, struct Eco_Fiber*);

#endif
