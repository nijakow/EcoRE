#ifndef ECO_OBJECTS_BASE_TYPE_TYPELIST_H
#define ECO_OBJECTS_BASE_TYPE_TYPELIST_H

struct Eco_Type;

struct Eco_TypeList
{
    struct Eco_Type**  types;
    unsigned int       alloc;
    unsigned int       fill;
};

void Eco_TypeList_Create(struct Eco_TypeList*);
void Eco_TypeList_Destroy(struct Eco_TypeList*);
void Eco_TypeList_Insert(struct Eco_TypeList*, struct Eco_Type*);
void Eco_TypeList_Remove(struct Eco_TypeList*, struct Eco_Type*);

#endif
