#ifndef ECO_OBJECTS_BASE_TYPE_TRANSFER_H
#define ECO_OBJECTS_BASE_TYPE_TRANSFER_H

struct Eco_Type;

struct Eco_TypeTransfer
{

};

void Eco_TypeTransfer_Create(struct Eco_TypeTransfer*);
void Eco_TypeTransfer_Destroy(struct Eco_TypeTransfer*);

void Eco_TypeTransfer_Commit(struct Eco_TypeTransfer*);

void Eco_TypeTransfer_AddSlotChange(struct Eco_TypeTransfer*, struct Eco_Type*, unsigned int, struct Eco_Type*);

#endif
