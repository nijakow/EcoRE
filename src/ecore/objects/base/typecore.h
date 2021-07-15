#pragma once

struct Eco_Object;
struct Eco_Message;
struct Eco_GC_State;

struct Eco_TypeCore
{
    bool (*send)(struct Eco_Message*, struct Eco_Object*);
    void (*mark)(struct Eco_GC_State*, struct Eco_Object*);
    void (*del)(struct Eco_Object*);
};
