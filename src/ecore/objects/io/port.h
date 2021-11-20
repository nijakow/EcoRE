#ifndef ECO_OBJECTS_IO_PORT_H
#define ECO_OBJECTS_IO_PORT_H

#include <ecore/eco.h>
#include <ecore/objects/base/object.h>

struct Eco_Fiber;
struct Eco_Scheduler;

#define Eco_Port_OUTPUT_BUFFER_SIZE 1024

struct Eco_Port
{
    struct Eco_Object      _;
    struct Eco_Scheduler*  scheduler;
    struct Eco_Port*       next;
    struct Eco_Fiber*      waiting_fiber;
    unsigned int           fd;
    unsigned int           output_buffer_fill;
    char                   output_buffer[Eco_Port_OUTPUT_BUFFER_SIZE];
};

struct Eco_Port* Eco_Port_New(struct Eco_Scheduler*, unsigned int);
void Eco_Port_Mark(struct Eco_GC_State*, struct Eco_Port*);
void Eco_Port_Del(struct Eco_Port*);

bool Eco_Port_FlushOutput(struct Eco_Port*);

bool Eco_Port_ReadBytes(struct Eco_Port*, char*, unsigned int);
bool Eco_Port_WriteBytes(struct Eco_Port*, char*, unsigned int);

bool Eco_Port_ReadChar(struct Eco_Port*, Eco_Codepoint*);
bool Eco_Port_WriteChar(struct Eco_Port*, Eco_Codepoint);

void Eco_Port_QueueForInput(struct Eco_Port*, struct Eco_Fiber*);

void Eco_Port_Init();
void Eco_Port_Terminate();

#endif
