#pragma once

struct Eco_Net_Sched;

struct Eco_Net_Sched* Eco_Net_Sched_New(unsigned int);
void Eco_Net_Sched_Delete(struct Eco_Net_Sched*);
void Eco_Net_Sched_Tick(struct Eco_Net_Sched*, int);
