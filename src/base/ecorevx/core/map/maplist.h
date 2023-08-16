#ifndef ECO_CORE_MAP_MAPLIST_H
#define ECO_CORE_MAP_MAPLIST_H

struct Eco_Map;

struct Eco_MapList
{
    unsigned int      alloc;
    unsigned int      fill;
    struct Eco_Map**  entries;
};

void Eco_MapList_Create(struct Eco_MapList*);
void Eco_MapList_Destroy(struct Eco_MapList*);

void Eco_MapList_Insert(struct Eco_MapList*, struct Eco_Map*);
void Eco_MapList_Remove(struct Eco_MapList*, struct Eco_Map*);

#endif
