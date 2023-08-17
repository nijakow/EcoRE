#ifndef ECO_VM_MEMORY_ARENA_H
#define ECO_VM_MEMORY_ARENA_H

struct Eco_Arena
{
    struct Eco_Object*  objects;
    unsigned int        object_count;
    
    /*
     * We keep these elements at the end to
     * speed up access to the "objects" element
     */
    struct Eco_Arena**  prev;
    struct Eco_Arena*   next;
};

void Eco_Arena_Create(struct Eco_Arena*);
void Eco_Arena_Destroy(struct Eco_Arena*);

#endif
