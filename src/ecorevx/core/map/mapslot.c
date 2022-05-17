#include "mapslot.h"


static void Eco_MapSlot_Create(struct Eco_MapSlot* self, struct Eco_SlotDef* def)
{
    self->def = *def;
}

void Eco_MapSlot_CreateInlined(struct Eco_MapSlot* self, struct Eco_SlotDef* def, unsigned int offset)
{
    Eco_MapSlot_Create(self, def);

    self->def.flags.is_code           = 0;
    self->body.inlined.offset         = offset;
    self->body.inlined.referenced_map = NULL;
}

void Eco_MapSlot_CreateCode(struct Eco_MapSlot* self, struct Eco_SlotDef* def, struct Eco_Object* code)
{
    Eco_MapSlot_Create(self, def);
}

void Eco_MapSlot_Destroy(struct Eco_MapSlot* self)
{
    /*
     * TODO: Notify the referenced_map that a reference was lost!
     */
}
