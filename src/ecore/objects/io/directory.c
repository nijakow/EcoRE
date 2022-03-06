#include <dirent.h>

#include "directory.h"

#include <ecore/objects/base/type.h>
#include <ecore/objects/misc/key/key.h>
#include <ecore/vm/core/send.h>
#include <ecore/util/memory.h>


/*
 *    D e c l a r a t i o n s
 */

bool Eco_Directory_Send(struct Eco_Message* message,
                        struct Eco_SendLink* link,
                        struct Eco_Object* target,
                        Eco_Any* self);


/*
 *    T y p e C o r e
 */

static struct Eco_TypeCore Eco_Directory_TYPECORE;
       struct Eco_Type*    Eco_Directory_TYPE;


void Eco_Directory_Init()
{
    Eco_TypeCore_Create(&Eco_Directory_TYPECORE, "Eco_Directory");

    Eco_Directory_TYPECORE.send = (Eco_TypeCore_SendFunc) Eco_Directory_Send;
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
    directory   = Eco_Object_New(Eco_Directory_TYPE, sizeof(struct Eco_Directory) + sizeof(char) * (path_length + 1));

    if (directory != NULL)
    {
        directory->path_length = path_length;
        Eco_Memcpy(directory->path, path, path_length);
        directory->path[path_length] = '\0';
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


/*
 *    T h e   S p i c y   P a r t
 */

bool Eco_Directory_Send(struct Eco_Message* message,
                        struct Eco_SendLink* link,
                        struct Eco_Object* target,
                        Eco_Any* self)
{
    struct Eco_Directory*  directory;
    DIR*                   d;
    struct dirent*         dir;

    directory = (struct Eco_Directory*) Eco_Any_AsPointer(self);
    
    d = opendir(directory->path);
    if (d != NULL)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (Eco_StrEq(message->key->name, dir->d_name)) {
                Eco_Log_Debug("%s\n", dir->d_name);
            }
        }
    }
    closedir(d);
    return false;
}
