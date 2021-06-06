#include "group.h"

#include "object.h"


bool Eco_Send_ToGroup(struct Eco_Message* message, struct Eco_Group* group)
{
    if (group->objects == NULL) return false;

    return Eco_Send_ToObject(message, group->objects[0]);
}
