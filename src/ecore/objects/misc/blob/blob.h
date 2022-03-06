#ifndef ECO_OBJECTS_MISC_BLOB_BLOB_H
#define ECO_OBJECTS_MISC_BLOB_BLOB_H

#include <ecore/eco.h>

#include <ecore/objects/base/object.h>

struct Eco_Blob
{
    struct Eco_Object  _;
    unsigned int       size;
    char               bytes[];
};

struct Eco_Blob* Eco_Blob_New(unsigned int);
void Eco_Blob_Mark(struct Eco_GC_State*, struct Eco_Blob*);
struct Eco_Blob* Eco_Blob_Clone(struct Eco_CloneState*, struct Eco_Blob*, bool);
void Eco_Blob_Del(struct Eco_Blob*);

static inline unsigned int Eco_Blob_Size(struct Eco_Blob* blob)
{
    return blob->size;
}

static inline char* Eco_Blob_At_Unchecked(struct Eco_Blob* blob, unsigned int index)
{
    return &blob->bytes[index];
}

static inline bool Eco_Blob_At(struct Eco_Blob* blob, unsigned int index, char *dst)
{
    *dst = *Eco_Blob_At_Unchecked(blob, index);
    return true;
}

void Eco_Blob_Init();
void Eco_Blob_Terminate();

#endif
