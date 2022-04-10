#include <ecore/vm/memory/memory.h>

#include "inheriting.h"

void Eco_Type_EstablishTypeLink(struct Eco_Type*     super,
                                struct Eco_TypeSlot* slot,
                                struct Eco_Type*     sub)
{
    struct Eco_TypeLink*  link;

    for (link = slot->body.inlined.referenced_types;
         link != NULL;
         link = link->right)
    {
        if (link->sub == sub)
            return;
    }

    link = Eco_Memory_Alloc(sizeof(struct Eco_TypeLink));

    if (link != NULL)
    {
        link->super            = super;
        link->slot             = slot;
        link->sub              = sub;
        link->right            = slot->body.inlined.referenced_types;
        slot->body.inlined.referenced_types = link;
        link->down             = sub->referencing_types;
        sub->referencing_types = link;
    }
}

static void Eco_TypeLink_Unlink(struct Eco_TypeLink* link)
{
    struct Eco_TypeLink**  ptr;

    ptr = &(link->slot->body.inlined.referenced_types);

    while (*ptr != NULL)
    {
        if (*ptr == link) {
            *ptr = link->right;
            break;
        }
        ptr = &((*ptr)->right);
    }

    ptr = &(link->sub->referencing_types);

    while (*ptr != NULL)
    {
        if (*ptr == link) {
            *ptr = link->down;
            break;
        }
        ptr = &((*ptr)->down);
    }
}

static void Eco_TypeLink_UnlinkAndFree(struct Eco_TypeLink* link)
{
    Eco_TypeLink_Unlink(link);
    Eco_Memory_Free(link);
}

void Eco_Type_RemoveAllTypeLinks(struct Eco_Type* type)
{
    unsigned int  index;

    while (type->referencing_types != NULL)
        Eco_TypeLink_UnlinkAndFree(type->referencing_types);
    
    for (index = 0; index < type->slot_count; index++)
    {
        if (type->slots[index].type == Eco_TypeSlotType_INLINED) {
            while (type->slots[index].body.inlined.referenced_types != NULL)
                Eco_TypeLink_UnlinkAndFree(type->slots[index].body.inlined.referenced_types);
        }
    }
}
