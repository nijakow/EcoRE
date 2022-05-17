#include "slotdef.h"


void Eco_SlotFlags_Create(struct Eco_SlotFlags* self)
{
    self->is_code      = 0;
    self->is_protected = 0;
    self->is_static    = 0;
    self->is_with      = 0;
    self->is_inherited = 0;
    self->is_own       = 0;
}

void Eco_SlotFlags_Destroy(struct Eco_SlotFlags* self)
{
}


void Eco_SlotDef_Create(struct Eco_SlotDef* self, struct Eco_Object* name)
{
    self->name = name;
    Eco_SlotFlags_Create(&self->flags);
}

void Eco_SlotDef_Destroy(struct Eco_SlotDef* self)
{
    Eco_SlotFlags_Destroy(&self->flags);
}
