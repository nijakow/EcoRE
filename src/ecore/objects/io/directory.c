#include "directory.h"

#include <ecore/objects/base/type.h>
#include <ecore/util/memory.h>


/*
 *    T y p e C o r e
 */

static struct Eco_TypeCore Eco_Directory_TYPECORE;
       struct Eco_Type*    Eco_Directory_TYPE;


void Eco_Directory_Init()
{
    Eco_TypeCore_Create(&Eco_Directory_TYPECORE, "Eco_Directory");

    Eco_Directory_TYPECORE.send = (Eco_TypeCore_SendFunc) Eco_Object_Send;
    Eco_Directory_TYPECORE.mark = (Eco_TypeCore_MarkFunc) Eco_Directory_Mark;
    Eco_Directory_TYPECORE.del  = (Eco_TypeCore_DelFunc)  Eco_Directory_Del;

    Eco_Directory_TYPE          = Eco_Type_NewPrefab(&Eco_Directory_TYPECORE);
}

void Eco_Directory_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_Directory_TYPECORE);
}


/*
 *    B a s i c s
 */

struct Eco_Directory* Eco_Directory_New(char* path)
{
    struct Eco_Directory*  directory;
    unsigned int           path_length;

    path_length = Eco_Strlen(path);
    directory   = Eco_Object_New(Eco_Directory_TYPE, sizeof(struct Eco_Directory) + sizeof(char) * path_length);

    if (directory != NULL)
    {
        directory->path_length = path_length;
        Eco_Memcpy(directory->path, path, path_length);
    }

    return directory;
}

void Eco_Directory_Mark(struct Eco_GC_State* state, struct Eco_Directory* directory)
{
    Eco_Object_Mark(state, &directory->_);
}

void Eco_Directory_Del(struct Eco_Directory* directory)
{
    Eco_Object_Del(&directory->_);
}
