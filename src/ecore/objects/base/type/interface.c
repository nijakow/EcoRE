#include "interface.h"

#include <ecore/base/extra.h>

#include <ecore/objects/base/typecore.h>
#include <ecore/vm/builtins/builtins.h>
#include <ecore/vm/memory/arena.h>

#include <ecore/objects/base/object.h>
#include <ecore/objects/base/molecule/molecule.h>
#include <ecore/objects/misc/key/key.h>
#include <ecore/objects/vm/code/code.h>
#include <ecore/objects/vm/code/closure.h>
#include <ecore/objects/vm/interface/interface.h>
#include <ecore/vm/core/clone.h>
#include <ecore/vm/core/send.h>
#include <ecore/vm/fiber/fiber.h>
#include <ecore/vm/fiber/stackops.h>
#include <ecore/vm/core/interpreter.h>
#include <ecore/vm/memory/memory.h>
#include <ecore/vm/memory/gc/gc.h>
#include <ecore/io/logging/log.h>


struct Eco_Interface* Eco_Any_GetInterface(Eco_Any value, bool private_also)
{
    return Eco_Type_GetInterface(Eco_Any_GetType(value), private_also);
}

struct Eco_Interface* Eco_Type_GetInterface(struct Eco_Type*   type,
                                            bool               private_also)
{
    struct Eco_Interface*  interface;
    unsigned int           index;
    unsigned int           parent_index;
    unsigned int           slot_index;
    unsigned int           inherited_slot_count;
    unsigned int           slot_count;

    /*
     * If the type has a proxy, use the proxy's interface instead.
     *
     * TODO, FIXME, XXX:
     * This is a very ugly fix. It ignores the slots already located
     * within the type, but it gives us an opportunity to get the
     * interface of e.g. strings and integers without too much hassle.
     *                                                - nijakow
     */
    if (type->proxy != NULL) {
        return Eco_Type_GetInterface(type->proxy->type, private_also);
    }

    /*
     * Check if we already have the interface pointer set.
     * If so, return its stored value.
     */
    if (private_also) {
        if (type->interface != NULL)
            return type->interface;
    } else {
        if (type->public_interface != NULL)
            return type->public_interface;
    }
    
    /*
     * The interface has not been computed yet, so we prepare ourselves
     * for creating a new interface instance. For that, we need to know
     * how many slots and parents it will have.
     * 
     * We therefore run a basic loop, incrementing the corresponding
     * counters.
     */
    inherited_slot_count = 0;
    slot_count           = 0;
    for (index = 0; index < type->slot_count; index++)
    {
        if (private_also || !type->slots[index].info.flags.is_protected) {
            if (type->slots[index].type == Eco_TypeSlotType_INLINED && type->slots[index].info.flags.is_with
            && type->slots[index].body.inlined.referenced_type != NULL && type->slots[index].body.inlined.referenced_type != type)
                inherited_slot_count++;
            slot_count++;
        }
    }

    /*
     * Everything's ready, we can now allocate.
     * 
     * TODO, FIXME, XXX: There is no check if the interface is NULL!
     */
    interface = Eco_Interface_New(inherited_slot_count, slot_count);

    /*
     * Update the interface pointers to point to our newly
     * created interface. We do this before setting up the
     * contents of our interface so that we can avoid
     * infinite recursion pitfalls.
     */
    if (private_also) {
        type->interface = interface;
        Eco_Interface_NotifyImplementedBy(interface, type);
    } else {
        type->public_interface = interface;
        Eco_Interface_NotifyImplementedBy(interface, type);
    }

    /*
     * Now we do the actual copying of the slot names and types
     * and all the other important stuff into the interface.
     */
    parent_index = 0;
    slot_index   = 0;
    for (index = 0; index < type->slot_count; index++)
    {
        if (!private_also && type->slots[index].info.flags.is_protected)
            continue;
        if (type->slots[index].type == Eco_TypeSlotType_INLINED && type->slots[index].info.flags.is_with) {
            /*
             * Check for circular inheritance (only valid if the slot points to SELF)
             */
            if (type->slots[index].body.inlined.referenced_type == NULL || type->slots[index].body.inlined.referenced_type == type) {
                /*
                 * Circular inheritance confirmed. Since an interface always includes itself,
                 * there is no need for us to add a parent entry.
                 */
            } else {
                interface->parents[parent_index++] = Eco_Type_GetInterface(type->slots[index].body.inlined.referenced_type, private_also);
            }
        }
        interface->parent_count = parent_index;
        interface->entries[slot_index].return_type = type->slots[index].interface;
        interface->entries[slot_index].key         = type->slots[index].info.key;
        interface->entries[slot_index].arg_count   = 0;      // TODO: If it's a method, check for args
        interface->entries[slot_index].has_varargs = false;  // TODO: If it's a method, check for varargs
        slot_index++;
    }

    /*
     * Done, return the interface.
     */
    return interface;
}
