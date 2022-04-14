#include <stdio.h>
#include <unistd.h>

#include <ecore/base/config.h>

#ifdef ECO_CONFIG_USE_DLOPEN
# include <dlfcn.h>
#endif

#include "blob.h"

#include <ecore/objects/base/type.h>
#include <ecore/objects/misc/string/string.h>
#include <ecore/vm/memory/memory.h>
#include <ecore/vm/memory/gc/gc.h>
#include <ecore/vm/core/clone.h>
#include <ecore/vm/core/send.h>
#include <ecore/util/memory.h>


/*
 *    T y p e
 */

static struct Eco_TypeCore Eco_Blob_TYPECORE;
       struct Eco_Type*    Eco_Blob_TYPE;


void Eco_Blob_Init()
{
    Eco_TypeCore_Create(&Eco_Blob_TYPECORE, "Eco_Blob");

    Eco_Blob_TYPECORE.send  = (Eco_TypeCore_SendFunc)  Eco_Object_Send;
    Eco_Blob_TYPECORE.mark  = (Eco_TypeCore_MarkFunc)  Eco_Blob_Mark;
    Eco_Blob_TYPECORE.clone = (Eco_TypeCore_CloneFunc) Eco_Blob_Clone;
    Eco_Blob_TYPECORE.del   = (Eco_TypeCore_DelFunc)   Eco_Blob_Del;

    Eco_Blob_TYPE           = Eco_Type_NewPrefab(&Eco_Blob_TYPECORE);
}

void Eco_Blob_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_Blob_TYPECORE);
}


/*
 *    B a s i c s
 */

struct Eco_Blob* Eco_Blob_New(unsigned int element_count)
{
    /*
     * In this case, the blob size is fixed (no swapout, no realloc),
     * so we can overallocate and make blob->bytes point to blob->payload.
     */
    struct Eco_Blob*  blob;

    blob = Eco_Object_New(Eco_Blob_TYPE, sizeof(struct Eco_Blob) + sizeof(char) * element_count);

    if (blob != NULL)
    {
        /*
         * The blob is not initialized by default. This could leak information,
         * but we don't worry about that now (TODO, XXX).
         */
        blob->size = element_count;
        blob->bytes = blob->payload;
    }

    return blob;
}

void Eco_Blob_Mark(struct Eco_GC_State* state, struct Eco_Blob* blob)
{
    Eco_Object_Mark(state, &blob->_);
}

struct Eco_Blob* Eco_Blob_Clone(struct Eco_CloneState* state,
                                struct Eco_Blob* blob,
                                bool forced)
{
    struct Eco_Blob*  the_clone;

    if (!forced) return blob;

    the_clone = Eco_Blob_New(Eco_Blob_Size(blob));
    
    Eco_CloneState_RegisterClone(state,
                                 (struct Eco_Object*) blob,
                                 (struct Eco_Object*) the_clone);

    Eco_Memcpy(the_clone->bytes, blob->bytes, Eco_Blob_Size(the_clone));

    return the_clone;
}

void Eco_Blob_Del(struct Eco_Blob* blob)
{
    Eco_Object_Del(&blob->_);
}


/*
 *    E l e m e n t   A c c e s s
 */

bool Eco_Blob_AtN(struct Eco_Blob* blob, unsigned int index, void* data, unsigned int size)
{
    if (index + size > Eco_Blob_Size(blob))
        return false;
    Eco_Memcpy(data, Eco_Blob_At_Unchecked(blob, index), size);
    return true;
}

bool Eco_Blob_AtPut(struct Eco_Blob* blob, unsigned int index, void* data, unsigned int size)
{
    if (index + size > Eco_Blob_Size(blob))
        return false;
    Eco_Memcpy(Eco_Blob_At_Unchecked(blob, index), data, size);
    return true;
}

struct Eco_Blob* Eco_Blob_NewFromData(void* data, unsigned long size)
{
    struct Eco_Blob*  blob;

    blob = Eco_Blob_New(size);

    if (blob != NULL)
    {
        Eco_Memcpy(blob->bytes, data, size);
    }

    return blob;
}

/*
 *    F i l e   D e s c r i p t o r   I / O
 */

int Eco_Blob_ReadFromFD(struct Eco_Blob* blob, int fd, unsigned long offset, unsigned long bytes)
{
    unsigned long size;

    size = (unsigned long) Eco_Blob_Size(blob);
    if (offset >= size) return 0;
    if (size < (offset + bytes)) bytes = size - offset;
    return read(fd, blob->bytes + offset, bytes);
}

int Eco_Blob_WriteToFD(struct Eco_Blob* blob, int fd, unsigned long offset, unsigned long bytes)
{
    unsigned long size;

    size = (unsigned long) Eco_Blob_Size(blob);
    if (offset >= size) return 0;
    if (size < (offset + bytes)) bytes = size - offset;
    return write(fd, blob->bytes + offset, bytes);
}


/*
 *    F i l e   M i r r o r i n g
 */

struct Eco_Blob* Eco_Blob_NewFromFile(char* path)
{
    struct Eco_Blob*  blob;
    FILE*             f;
    unsigned int      size;

    f = fopen(path, "r");

    if (f != NULL) {
        fseek(f, 0L, SEEK_END);
        size = (unsigned int) ftell(f); // TODO, FIXME, XXX: unsigned int might be too small!
        fseek(f, 0L, SEEK_SET);
        blob = Eco_Blob_New(size);
        if (blob != NULL)
            fread(blob->bytes, sizeof(char), size, f);
        fclose(f);
    } else {
        blob = Eco_Blob_New(0);
    }

    return blob;
}

struct Eco_Blob* Eco_Blob_DLOpen(char* path)
{
#ifdef ECO_CONFIG_USE_DLOPEN
    void* ptr;

    ptr = dlopen(path, RTLD_NOW);
    if (ptr == NULL)
        return NULL;
    return Eco_Blob_NewFromData(&ptr, sizeof(ptr));
#else
    return NULL;
#endif
}

struct Eco_Blob* Eco_Blob_DLSym(void* base, char* symbol)
{
#ifdef ECO_CONFIG_USE_DLOPEN
    void* symptr;

    symptr = dlsym(base, symbol);
    if (symptr == NULL)
        return NULL;
    return Eco_Blob_NewFromData(&symptr, sizeof(symptr));
#else
    return NULL;
#endif
}
