#include <ecore/eco.h>

#include "typecore.h"

#include <ecore/objects/base/object.h>


void Eco_TypeCore_Create(struct Eco_TypeCore* self, const char* name)
{
    self->del           = NULL;
    self->mark_instance = NULL;
    self->mark_children = NULL;
    self->clone         = Eco_Object_NoClone;
    self->send          = Eco_Object_Send;

    self->name  = name;
}


void Eco_TypeCore_Destroy(struct Eco_TypeCore* self)
{
}
