#include "copying.h"

#include <ecore/base/extra.h>

#include <ecore/objects/base/typecore.h>
#include <ecore/vm/builtins/builtins.h>
#include <ecore/vm/memory/arena.h>

#include <ecore/objects/base/object.h>
#include <ecore/objects/molecule/molecule.h>
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


struct Eco_Type* Eco_Type_New(unsigned int slot_count);


static bool Eco_Type_CopyWithNewSlot(struct Eco_Type*      self,
                                     int                   pos,
                                     struct Eco_Type**     type_loc,
                                     struct Eco_TypeSlot** slot_loc)
{
    unsigned int      i;
    unsigned int      adjusted_pos;
    struct Eco_Type*  the_copy;

    const unsigned int  new_slot_count = self->slot_count + 1;

    if (pos >= 0) adjusted_pos =  pos;
    else          adjusted_pos = self->slot_count + pos + 1;

    if (adjusted_pos >= new_slot_count) adjusted_pos = new_slot_count - 1;


    the_copy                        = Eco_Type_New(new_slot_count);
    *type_loc                       = the_copy;
    the_copy->typecore              = self->typecore;
    the_copy->instance_payload_size = self->instance_payload_size;

    for (i = 0; i < new_slot_count; i++) {
        if (i < adjusted_pos) the_copy->slots[i] = self->slots[i];
        else if (i == adjusted_pos) {
            Eco_TypeSlot_Initialize(&the_copy->slots[i]);
            *slot_loc = &the_copy->slots[i];
        }
        else the_copy->slots[i] = self->slots[i - 1];
    }

    *type_loc = the_copy;

    return true;
}

bool Eco_Type_CopyWithNewInlinedSlot(struct Eco_Type*           self,
                                     int                        pos,
                                     struct Eco_BasicSlotInfo*  info,
                                     struct Eco_Interface*      interface,
                                     struct Eco_Type**          type_loc,
                                     struct Eco_TypeSlot**      slot_loc)
{
    struct Eco_Type*      the_copy;
    struct Eco_TypeSlot*  the_slot;

    const unsigned int     slot_value_size  = sizeof(Eco_Any);

    the_slot                                = NULL;

    if (Eco_Type_CopyWithNewSlot(self, pos, &the_copy, &the_slot)) {
        the_copy->instance_payload_size += slot_value_size;

        the_slot->type                    = Eco_TypeSlotType_INLINED;
        the_slot->info                    = *info;
        the_slot->interface               = interface;
        the_slot->body.inlined.value_size = slot_value_size;
        the_slot->body.inlined.offset     = the_copy->instance_payload_size - slot_value_size;

        *type_loc                         = the_copy;
        *slot_loc                         = the_slot;
        
        return true;
    } else {
        return false;
    }
}

bool Eco_Type_CopyWithNewCodeSlot(struct Eco_Type*          self,
                                  int                       pos,
                                  struct Eco_BasicSlotInfo* info,
                                  struct Eco_Interface*     interface,
                                  struct Eco_Code*          code,
                                  struct Eco_Type**         type_loc)
{
    struct Eco_Type*       the_copy;
    struct Eco_TypeSlot*  the_slot;

    const unsigned int     slot_value_size  = sizeof(Eco_Any);
    the_slot                                = NULL;

    if (Eco_Type_CopyWithNewSlot(self, pos, &the_copy, &the_slot)) {
        the_copy->instance_payload_size += slot_value_size;

        the_slot->type             = Eco_TypeSlotType_CODE;
        the_slot->info             = *info;
        the_slot->interface        = interface;
        the_slot->body.code.code   = code;

        *type_loc                = the_copy;

        return true;
    } else {
        return false;
    }
}

bool Eco_Type_CopyWithRemovedSlot(struct Eco_Type* self,
                                  unsigned int pos,
                                  struct Eco_Type** type_loc)
{
    unsigned int      i;
    unsigned int      adjusted_pos;
    struct Eco_Type*  the_copy;

    const unsigned int  new_slot_count = self->slot_count - 1;

    if (pos >= self->slot_count) adjusted_pos = self->slot_count - 1;
    else                         adjusted_pos = pos;

    the_copy                        = Eco_Type_New(new_slot_count);
    *type_loc                       = the_copy;
    the_copy->typecore              = self->typecore;
    the_copy->instance_payload_size = self->instance_payload_size;

    for (i = 0; i < self->slot_count; i++) {
        if (i < adjusted_pos) the_copy->slots[i] = self->slots[i];
        else if (i == adjusted_pos) continue;
        else the_copy->slots[i - 1] = self->slots[i];
    }

    *type_loc = the_copy;

    return true;
}
