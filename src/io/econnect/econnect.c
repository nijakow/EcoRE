#include "econnect.h"


typedef void (*Eco_EConnect_Handler_Func)(); /* TODO */
typedef unsigned int Eco_EConnect_ID;


struct Eco_EConnect_RefInfo
{
    struct Eco_EConnect_RefInfo*  next;
    Eco_EConnect_ID               id;
    struct Eco_Object*            object;
    Eco_EConnect_Handler_Func     handler_function;
};

