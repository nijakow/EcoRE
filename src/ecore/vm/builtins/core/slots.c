#include "slots.h"

#include <ecore/objects/base/molecule/object_slots.h>
#include <ecore/objects/base/molecule/molecule.h>

/*
 * TODO, FIXME, XXX!
 *
 * This function has multiple weaknesses:
 *   - It doesn't check for the argument types
 *   - AddSlot might trigger a GC that collects the arguments before they can be used
 *   - The return value is set in an inefficient way
 */
bool Eco_VM_Builtin_AddValueSlot(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any                   object;
    Eco_Any                   index;
    Eco_Any                   flags;
    Eco_Any                   type;
    Eco_Any                   key;
    Eco_Any                   value;
    struct Eco_BasicSlotInfo  info;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 6, 6))
        return false;
    
    Eco_Fiber_Pop(fiber, &value);
    Eco_Fiber_Pop(fiber, &key);
    Eco_Fiber_Pop(fiber, &type);
    Eco_Fiber_Pop(fiber, &flags);
    Eco_Fiber_Pop(fiber, &index);
    Eco_Fiber_Pop(fiber, &object);

    // TODO: More flags
    info.key                =  (struct Eco_Key*) Eco_Any_AsPointer(key);
    info.flags.is_inherited =  (Eco_Any_AsInteger(flags) & 0x01) == 0;
    info.flags.is_with      =  (Eco_Any_AsInteger(flags) & 0x02) != 0;
    info.flags.is_part      =  (Eco_Any_AsInteger(flags) & 0x04) != 0;
    info.flags.is_private   =  (Eco_Any_AsInteger(flags) & 0x08) != 0;

    Eco_Molecule_AddSlot((struct Eco_Molecule*) Eco_Any_AsPointer(object),
                         Eco_Any_AsInteger(index),
                         &info,
                         (struct Eco_Interface*) Eco_Any_AsPointer(type),
                         value);

    Eco_Fiber_Push(fiber, &object);

    return true;
}

/*
 * TODO, FIXME, XXX!
 *
 * The same notes as for Eco_VM_Builtin_AddValueSlot(...) apply for
 * this function too!
 */
bool Eco_VM_Builtin_AddCodeSlot(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any                   object;
    Eco_Any                   index;
    Eco_Any                   flags;
    Eco_Any                   type;
    Eco_Any                   key;
    Eco_Any                   code;
    struct Eco_BasicSlotInfo  info;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 6, 6))
        return false;
    
    Eco_Fiber_Pop(fiber, &code);
    Eco_Fiber_Pop(fiber, &key);
    Eco_Fiber_Pop(fiber, &type);
    Eco_Fiber_Pop(fiber, &flags);
    Eco_Fiber_Pop(fiber, &index);
    Eco_Fiber_Pop(fiber, &object);

    // TODO: More flags
    info.key                =  (struct Eco_Key*) Eco_Any_AsPointer(key);
    info.flags.is_inherited =  (Eco_Any_AsInteger(flags) & 0x01) == 0;
    info.flags.is_with      =  (Eco_Any_AsInteger(flags) & 0x02) != 0;
    info.flags.is_part      =  (Eco_Any_AsInteger(flags) & 0x04) != 0;
    info.flags.is_private   =  (Eco_Any_AsInteger(flags) & 0x08) != 0;

    Eco_Molecule_AddCodeSlot((struct Eco_Molecule*) Eco_Any_AsPointer(object),
                             Eco_Any_AsInteger(index),
                             &info,
                             (struct Eco_Interface*) Eco_Any_AsPointer(type),
                             (struct Eco_Code*) Eco_Any_AsPointer(code));

    Eco_Fiber_Push(fiber, &object);

    return true;
}

bool Eco_VM_Builtin_RemoveSlot(struct Eco_Fiber* fiber, unsigned int args)
{
    Eco_Any                     object;
    Eco_Any                     index;

    if (!Eco_VM_Builtin_Tool_ArgExpect(fiber, args, 2, 2))
        return false;
    
    Eco_Fiber_Pop(fiber, &index);
    Eco_Fiber_Pop(fiber, &object);

    Eco_Molecule_RemoveSlot((struct Eco_Molecule*) Eco_Any_AsPointer(object),
                            Eco_Any_AsInteger(index));

    Eco_Fiber_Push(fiber, &object);

    return true;
}
