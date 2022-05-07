#include <ecore/io/logging/log.h>
#include <ecore/util/libc.h>

#include "typelist.h"

void Eco_TypeList_Create(struct Eco_TypeList* list)
{
    list->types = NULL;
    list->alloc = 0;
    list->fill  = 0;
}

void Eco_TypeList_Destroy(struct Eco_TypeList* list)
{
    if (list->types != NULL)
        Eco_LibC_Free(list->types);
}

void Eco_TypeList_Insert(struct Eco_TypeList* list, struct Eco_Type* type)
{
    unsigned int index;

    for (index = 0; index < list->fill; index++)
    {
        if (list->types[index] == type)
            return;
    }

    if (list->fill >= list->alloc)
    {
        list->types = Eco_LibC_Realloc(list->types, (list->fill + 8) * sizeof(struct Eco_Type*));
        list->alloc = list->fill + 8;
        // TODO: Check for NULL
    }
    list->types[list->fill++] = type;
}

void Eco_TypeList_Remove(struct Eco_TypeList* list, struct Eco_Type* type)
{
    unsigned int  index;

    for (index = 0; index < list->fill; index++)
    {
        if (list->types[index] == type)
        {
            while (index < list->fill - 1)
            {
                list->types[index] = list->types[index + 1];
                index++;
            }
            list->fill--;
        }
    }
}
