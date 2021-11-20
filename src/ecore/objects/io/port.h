#ifndef ECO_OBJECTS_IO_PORT_H
#define ECO_OBJECTS_IO_PORT_H

#include <ecore/eco.h>
#include <ecore/objects/base/object.h>

struct Eco_Fiber;
struct Eco_Scheduler;

#define Eco_Port_INPUT_BUFFER_SIZE  1024
#define Eco_Port_OUTPUT_BUFFER_SIZE 1024

struct Eco_Port
{
    struct Eco_Object      _;
    struct Eco_Scheduler*  scheduler;
    struct Eco_Port*       next;
    struct Eco_Fiber*      waiting_fiber;
    void                 (*waiting_fiber_func)(struct Eco_Fiber*, struct Eco_Port*);
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

void Eco_Port_Read(struct Eco_Port*);
bool Eco_Port_FlushOutput(struct Eco_Port*);

bool Eco_Port_ReadByte(struct Eco_Port*, char*);
bool Eco_Port_WriteBytes(struct Eco_Port*, char*, unsigned int);

bool Eco_Port_WriteChar(struct Eco_Port*, Eco_Codepoint);

void Eco_Port_Reactivate(struct Eco_Port*);

void Eco_Port_Init();
void Eco_Port_Terminate();

#endif
