#include "group.h"

#include "object.h"
#include "type.h"
#include "../vm/memory/memory.h"
#include "../vm/memory/gc/gc.h"


struct Eco_Group* Eco_Group_New(struct Eco_Object* core)
{
    struct Eco_Group* group;

    group = Eco_Object_New_Derived(Eco_Type_GROUP_TYPE, sizeof(struct Eco_Group), 0);

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
    struct Eco_Object**  objects;
    unsigned int         count;
    unsigned int         alloc;
    unsigned int         i;

    Eco_Group_GetMembers(group, &objects, &count, &alloc);

    for (i = 0; i < count; i++)
    {
        Eco_GC_State_MarkObject(state, objects[i]);
    }

    Eco_Object_Mark(state, &(group->_));
}

void Eco_Group_Del(struct Eco_Group* group)
{
    if (group->object_alloc > 0) {
        Eco_Memory_Free(group->body.multi.objects);
    }
    Eco_Object_Del(&(group->_));
}
