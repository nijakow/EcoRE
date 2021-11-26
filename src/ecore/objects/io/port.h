#ifndef ECO_OBJECTS_IO_PORT_H
#define ECO_OBJECTS_IO_PORT_H

#include <ecore/eco.h>
#include <ecore/objects/base/object.h>
#include <ecore/vm/fiber/sched.h>

struct Eco_Scheduler;

#define Eco_Port_INPUT_BUFFER_SIZE  1024
#define Eco_Port_OUTPUT_BUFFER_SIZE 1024

struct Eco_Port
{
    struct Eco_Object      _;
    struct Eco_Scheduler*  scheduler;
    struct Eco_Port*       next;
    struct Eco_FiberQueue  fibers;
    unsigned int           fd;
    unsigned int           input_buffer_read_head;
    unsigned int           input_buffer_fill;
    char                   input_buffer[Eco_Port_INPUT_BUFFER_SIZE];
    unsigned int           output_buffer_fill;
    char                   output_buffer[Eco_Port_OUTPUT_BUFFER_SIZE];
};

struct Eco_Port* Eco_Port_New(struct Eco_Scheduler*, unsigned int);
void Eco_Port_Mark(struct Eco_GC_State*, struct Eco_Port*);
void Eco_Port_Del(struct Eco_Port*);

bool Eco_Port_FlushOutput(struct Eco_Port*);

bool Eco_Port_HasInput(struct Eco_Port*);
bool Eco_Port_HasOutput(struct Eco_Port*);

bool Eco_Port_ReadByte(struct Eco_Port*, char*);
bool Eco_Port_WriteByte(struct Eco_Port*, char);
bool Eco_Port_WriteBytes(struct Eco_Port*, char*, unsigned int);
bool Eco_Port_WriteChar(struct Eco_Port*, Eco_Codepoint);

void Eco_Port_Reactivate(struct Eco_Port*);
void Eco_Port_RequestUpdate(struct Eco_Port*);
bool Eco_Port_QueueFiber(struct Eco_Port*, struct Eco_Fiber*);

void Eco_Port_Init();
void Eco_Port_Terminate();

#endif
