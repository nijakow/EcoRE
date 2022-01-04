#include "interface.h"

#include <ecore/objects/base/type.h>

#include <ecore/vm/memory/memory.h>
#include <ecore/vm/memory/gc/gc.h>


/*
 *    T y p e
 */

static struct Eco_TypeCore  Eco_Interface_TYPECORE;
       struct Eco_Type*     Eco_Interface_TYPE;


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

static struct Eco_Interface* Eco_INTERFACES        = NULL;
static struct Eco_Interface* Eco_Interface_DEFAULT = NULL;


struct Eco_Interface* Eco_Interface_New(unsigned int entry_count)
{
    struct Eco_Interface*  interface;
    unsigned int           i;

    interface = Eco_Object_New(Eco_Interface_TYPE, sizeof(struct Eco_Interface) + sizeof(struct Eco_InterfaceEntry) * entry_count);

    if (interface != NULL)
    {
        interface->allow_all   = false;
        interface->entry_count = entry_count;
        for (i = 0; i < entry_count; i++)
            interface->entries[i].key = NULL;   // TODO: Use a sentinel for this to speed up marking
        /*
         * Link the interface into the global list of interfaces
         */
        {
            interface->prev = &Eco_INTERFACES;
            interface->next =  Eco_INTERFACES;
            if (Eco_INTERFACES != NULL)
                Eco_INTERFACES->prev = &interface->next;
            Eco_INTERFACES = interface;
        }
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
        entry = &interface->entries[i];
        if (entry != NULL && entry->key != NULL)
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
    if (interface->next != NULL)
        interface->next->prev = interface->prev;
    *interface->prev = interface->next;
    Eco_Object_Del(&(interface->_));
}

struct Eco_Interface* Eco_Interface_GetDefaultInterface()
{
    if (Eco_Interface_DEFAULT == NULL) {
        Eco_Interface_DEFAULT = Eco_Interface_New(0);
        Eco_Interface_DEFAULT->allow_all = true;
    }
    return Eco_Interface_DEFAULT;
}

/*
 *    M o d i f i c a t i o n
 */

void Eco_Interface_EstablishSuperSubRelation(struct Eco_Interface* super_interface,
                                             struct Eco_Interface* sub_interface)
{
    /*
     *    TODO: Create a link between these two instances
     */
}

struct Eco_Interface* Eco_Interface_NewAndInit(unsigned int size,
                                               struct Eco_InterfaceEntry* entries)
{
    unsigned int           index;
    struct Eco_Interface*  interface;

    interface = Eco_Interface_New(size);

    if (interface != NULL)
    {
        for (index = 0; index < size; index++)
            interface->entries[index] = entries[index];
    }

    return interface;
}

void Eco_Interface_AddParent(struct Eco_Interface* interface,
                             struct Eco_Interface* parent)
{
    // TODO
}

struct Eco_Interface* Eco_Interface_AddEntry(struct Eco_Interface* old_interface,
                                             struct Eco_InterfaceEntry* entry)
{
    struct Eco_Interface*  new_interface;
    unsigned int           i;

    new_interface = Eco_Interface_New(old_interface->entry_count + 1);

    if (new_interface != NULL)
    {
        for (i = 0; i < old_interface->entry_count; i++)
            new_interface->entries[i] = old_interface->entries[i];
        new_interface->entries[i] = *entry;
        Eco_Interface_EstablishSuperSubRelation(old_interface, new_interface);
        /*
         * TODO: Look for other interfaces that are supers to new_interface
         *       and establish the same relation.
         */
    }

    return new_interface;
}
