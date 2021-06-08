#include "group.h"

#include "object.h"
#include "../vm/memory.h"


struct Eco_Group* Eco_Group_New(struct Eco_Object* core)
{
    struct Eco_Group* group;

    group = Eco_Object_New(NULL, sizeof(struct Eco_Group)); /* TODO: Type */

    group->object_alloc    = 0;
    group->body.single[0]  = core;
    group->body.single[1]  = NULL;

    return group;
}


bool Eco_Send_ToGroup(struct Eco_Message* message, struct Eco_Group* group)
{
    if (group->object_alloc == 0) {
        return Eco_Send_ToObject(message, group->body.single[0]);
    } else {
        return Eco_Send_ToObject(message, group->body.multi.objects[0]);
    }
}
