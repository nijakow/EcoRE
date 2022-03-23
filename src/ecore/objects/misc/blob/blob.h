#ifndef ECO_OBJECTS_MISC_BLOB_BLOB_H
#define ECO_OBJECTS_MISC_BLOB_BLOB_H

#include <ecore/eco.h>

#include <ecore/objects/base/object.h>

struct Eco_Blob
{
    struct Eco_Object  _;
    unsigned int       size;
    char*              bytes;
    char               payload[];
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

static inline bool Eco_Blob_AtPutInt8(struct Eco_Blob* blob, unsigned int index, char value)
{
    *Eco_Blob_At_Unchecked(blob, index) = value;
    return true;
}

bool Eco_Blob_AtN(struct Eco_Blob* blob, unsigned int index, void* data, unsigned int size);
bool Eco_Blob_AtPut(struct Eco_Blob* blob, unsigned int index, void* data, unsigned int size);

int Eco_Blob_ReadFromFD(struct Eco_Blob*, int);
int Eco_Blob_WriteToFD(struct Eco_Blob*, int);

struct Eco_Blob* Eco_Blob_NewFromFile(char*);

void Eco_Blob_Init();
void Eco_Blob_Terminate();

#endif
