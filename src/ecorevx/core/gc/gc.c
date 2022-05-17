#include "../object/object.h"

#include "gc.h"
#include "tags.h"


void Eco_GC_UpdatePointer(struct Eco_Object** ptr)
{
    if ((*ptr)->header.gc_bits == Eco_GC_Tag_MOVED)
        *ptr = Eco_Any_AsPointer(Eco_Object_Get(*ptr, 0));
}

void Eco_GC_UpdateAny(Eco_Any* any)
{
    struct Eco_Object*  ptr;

    if (Eco_Any_IsPointer(*any))
    {
        ptr = Eco_Any_AsPointer(*any);
        Eco_GC_UpdatePointer(&ptr);
        *any = Eco_Any_FromPointer(ptr);
    }
}
