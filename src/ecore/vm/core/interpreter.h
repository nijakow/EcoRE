#pragma once

#include "../../eco.h"


struct Eco_Fiber;
struct Eco_Message;
struct Eco_Code;
struct Eco_Environment;


bool Eco_Fiber_Enter(struct Eco_Fiber*, struct Eco_Frame*, struct Eco_Message*, struct Eco_Code*);

void Eco_Fiber_Run(struct Eco_Fiber*);
