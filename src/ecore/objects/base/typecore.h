#pragma once

struct Eco_Object;
struct Eco_Message;
struct Eco_GC_State;


struct Eco_TypeCore
{
    bool (*send)(struct Eco_Message*, struct Eco_Object*);
    void (*mark)(struct Eco_GC_State*, struct Eco_Object*);
    void (*del)(struct Eco_Object*);

    const char*  name;
};


void Eco_TypeCore_Create(struct Eco_TypeCore*, const char*);
void Eco_TypeCore_Destroy(struct Eco_TypeCore*);
