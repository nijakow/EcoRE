#pragma once

struct Eco_Object_Payload
{
    unsigned int  size;
    char          data[];
};

struct Eco_Object_Payload* Eco_Object_Payload_New(unsigned int);
struct Eco_Object_Payload* Eco_Object_Payload_Resize(struct Eco_Object_Payload*, unsigned int);
void                       Eco_Object_Payload_Delete(struct Eco_Object_Payload*);
