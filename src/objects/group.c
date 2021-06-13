#include "group.h"

#include "object.h"
#include "type.h"
#include "../vm/memory/memory.h"



struct Eco_Group* Eco_Group_New(struct Eco_Object* core)
{
    struct Eco_Group* group;

    group = Eco_Object_New(Eco_Type_GROUP_TYPE, sizeof(struct Eco_Group));

    group->object_alloc    = 0;
    group->body.single[0]  = core;
    group->body.single[1]  = NULL;

    return group;
}

bool Eco_Group_Send(struct Eco_Message* message, struct Eco_Group* group)
{
    if (group->object_alloc == 0) {
        return Eco_Object_Send(message, group->body.single[0]);
    } else {
        return Eco_Object_Send(message, group->body.multi.objects[0]);
    }
}

void Eco_Group_Mark(struct Eco_GC_State* state, struct Eco_Group* group)
{
    Eco_Object_Mark(state, &(group->_));
}

void Eco_Group_Del(struct Eco_Group* group)
{
    if (group->object_alloc > 0) {
        Eco_Memory_Free(group->body.multi.objects);
    }
    Eco_Object_Del(&(group->_));
}

