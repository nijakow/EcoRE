#include "../../eco.h"

#include "typecore.h"


void Eco_TypeCore_Create(struct Eco_TypeCore* self, const char* name)
{
    self->del  = NULL;
    self->mark = NULL;
    self->send = NULL;

    self->name = name;
}


void Eco_TypeCore_Destroy(struct Eco_TypeCore* self)
{
}
