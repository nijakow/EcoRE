#include "eco.h"

void Eco_Create(struct Eco* self)
{
    Eco_Heap_Create(&self->heap);
}

void Eco_Destroy(struct Eco* self)
{
    Eco_Heap_Destroy(&self->heap);
}
