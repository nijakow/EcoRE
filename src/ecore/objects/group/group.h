#ifndef ECO_OBJECTS_GROUP_GROUP_H
#define ECO_OBJECTS_GROUP_GROUP_H

#include <ecore/objects/base/object.h>


struct Eco_Message;
struct Eco_SendLink;
struct Eco_GC_State;


struct Eco_Group
{
    struct Eco_Object    _;

    unsigned int         object_alloc;
    union {
        struct {
            unsigned int         object_count;
            struct Eco_Object**  objects;
        } multi;
        struct Eco_Object* single[2];
    } body;
};


static inline void Eco_Group_GetMembers(struct Eco_Group*     group,
                                        struct Eco_Object***  objects_dest,
                                        unsigned int*         size_dest,
                                        unsigned int*         alloc_dest)
{
    if (group->object_alloc == 0) {
        *objects_dest = group->body.single;
        if (group->body.single[0] == NULL) *size_dest = 0;
        else if (group->body.single[0] == NULL) *size_dest = 1;
        else *size_dest = 2;
        *alloc_dest = 2;
    } else {
        *objects_dest = group->body.multi.objects;
        *size_dest    = group->body.multi.object_count;
        *alloc_dest   = group->object_alloc;
    }
}


struct Eco_Group* Eco_Group_New();
bool Eco_Group_Send(struct Eco_Message*, struct Eco_SendLink*, struct Eco_Group*);
void Eco_Group_Mark(struct Eco_GC_State*, struct Eco_Group*);
void Eco_Group_Del(struct Eco_Group*);

void Eco_Group_AddObject(struct Eco_Group*, struct Eco_Object*);

void Eco_Group_Init();
void Eco_Group_Terminate();

#endif
