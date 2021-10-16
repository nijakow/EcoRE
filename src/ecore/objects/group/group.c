#include "group.h"

#include <ecore/objects/base/object.h>
#include <ecore/objects/base/type.h>
#include <ecore/objects/base/typecore.h>
#include <ecore/vm/memory/memory.h>
#include <ecore/vm/memory/gc/gc.h>


/*
 *    T y p e
 */

static struct Eco_TypeCore Eco_Group_TYPECORE;
static struct Eco_Type*    Eco_Group_TYPE;


void Eco_Group_Init()
{
    Eco_TypeCore_Create(&Eco_Group_TYPECORE, "Eco_Group");

    Eco_Group_TYPECORE.send = (Eco_TypeCore_SendFunc) Eco_Group_Send;
    Eco_Group_TYPECORE.mark = (Eco_TypeCore_MarkFunc) Eco_Group_Mark;
    Eco_Group_TYPECORE.del  = (Eco_TypeCore_DelFunc) Eco_Group_Del;

    Eco_Group_TYPE          = Eco_Type_NewPrefab(&Eco_Group_TYPECORE);
}

void Eco_Group_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_Group_TYPECORE);
}



/*
 *    B a s i c s
 */

struct Eco_Group* Eco_Group_New()
{
    struct Eco_Group* group;

    group = Eco_Object_New_Derived(Eco_Group_TYPE, sizeof(struct Eco_Group), 0);

    group->object_alloc    = 0;
    group->body.single[0]  = NULL;
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


void Eco_Group_AddObject(struct Eco_Group* group, struct Eco_Object* object)
{
    // TODO, FIXME, XXX: Stub
}
