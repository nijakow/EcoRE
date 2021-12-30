#include "interface.h"

#include <ecore/objects/base/type.h>

#include <ecore/vm/memory/memory.h>
#include <ecore/vm/memory/gc/gc.h>


/*
 *    T y p e
 */

static struct Eco_TypeCore Eco_Interface_TYPECORE;
static struct Eco_Type*    Eco_Interface_TYPE;


void Eco_Interface_Init()
{
    Eco_TypeCore_Create(&Eco_Interface_TYPECORE, "Eco_Interface");

    Eco_Interface_TYPECORE.send = (Eco_TypeCore_SendFunc) Eco_Object_Send;
    Eco_Interface_TYPECORE.mark = (Eco_TypeCore_MarkFunc) Eco_Interface_Mark;
    Eco_Interface_TYPECORE.del  = (Eco_TypeCore_DelFunc) Eco_Interface_Del;

    Eco_Interface_TYPE          = Eco_Type_NewPrefab(&Eco_Interface_TYPECORE);
}

void Eco_Interface_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_Interface_TYPECORE);
}


/*
 *    B a s i c s
 */

struct Eco_Interface* Eco_Interface_New(unsigned int entry_count)
{
    struct Eco_Interface*  interface;
    unsigned int           i;

    interface = Eco_Object_New(Eco_Interface_TYPE, sizeof(struct Eco_Interface) + sizeof(struct Eco_InterfaceEntry*) * entry_count);

    if (interface != NULL)
    {
        interface->entry_count = entry_count;
        for (i = 0; i < entry_count; i++)
            interface->entries[i] = NULL;   // TODO: Use a sentinel for this to speed up marking
    }

    return interface;
}

void Eco_Interface_Mark(struct Eco_GC_State* state, struct Eco_Interface* interface)
{
    struct Eco_InterfaceEntry*  entry;
    unsigned int                i;
    unsigned int                j;

    for (i = 0; i < interface->entry_count; i++)
    {
        entry = interface->entries[i];
        if (entry != NULL)
        {
            Eco_GC_State_MarkObject(state, (struct Eco_Object*) entry->key);
            Eco_GC_State_MarkObject(state, (struct Eco_Object*) entry->return_type);
            for (j = 0; j < entry->arg_count; j++)
            {
                Eco_GC_State_MarkObject(state, (struct Eco_Object*) entry->arg_types[j]);
            }
        }
    }

    Eco_Object_Mark(state, &(interface->_));
}

void Eco_Interface_Del(struct Eco_Interface* interface)
{
    unsigned int  i;

    for (i = 0; i < interface->entry_count; i++)
    {
        if (interface->entries[i] != NULL)
            Eco_Memory_Free(interface->entries[i]);
    }
    Eco_Object_Del(&(interface->_));
}
