#ifndef ECO_VM_CORE_INTERPRETER_H
#define ECO_VM_CORE_INTERPRETER_H

#include <ecore/eco.h>


struct Eco_Fiber;
struct Eco_Message;
struct Eco_Code;
struct Eco_Environment;


bool Eco_Fiber_EnterThunk(struct Eco_Fiber*, Eco_Any*, struct Eco_Code*);
bool Eco_Fiber_Enter(struct Eco_Fiber*, struct Eco_Frame*, struct Eco_Code*, unsigned int);

void Eco_Fiber_Run(struct Eco_Fiber*);

#endif
