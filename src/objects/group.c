#include "group.h"

#include "object.h"
#include "type.h"
#include "../vm/memory/memory.h"



bool Eco_Send_ToGroup(struct Eco_Message* message, struct Eco_Group* group)
{
    if (group->object_alloc == 0) {
        return Eco_Send_ToObject(message, group->body.single[0]);
    } else {
        return Eco_Send_ToObject(message, group->body.multi.objects[0]);
    }
}


void Eco_Group_MarkSlots(struct Eco_GC_State* state, struct Eco_Group* group)
{
    /* TODO */
}


void Eco_Group_Del(struct Eco_Group* group)
{
    if (group->object_alloc > 0) {
        Eco_Memory_Free(group->body.multi.objects);
    }
}



TYPE_DEFINITION(Eco_Group, Eco_Send_ToGroup, Eco_Group_MarkSlots, Eco_Group_Del, 0, {});



struct Eco_Group* Eco_Group_New(struct Eco_Object* core)
{
    struct Eco_Group* group;

    group = Eco_Object_New(&Eco_Group_Type, sizeof(struct Eco_Group));

    group->object_alloc    = 0;
    group->body.single[0]  = core;
    group->body.single[1]  = NULL;

    return group;
}
