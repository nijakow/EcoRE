#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include "libc.h"

void* Eco_LibC_Alloc(Eco_SizeT size)
{
    return malloc(size);
}

void* Eco_LibC_Realloc(void* ptr, Eco_SizeT size)
{
    if (ptr == NULL)
        return Eco_LibC_Alloc(size);
    return realloc(ptr, size);
}

void Eco_LibC_Free(void* ptr)
{
    free(ptr);
}

char* Eco_LibC_Strdup(const char* str)
{
    return strdup(str);
}

char* Eco_LibC_GetEnv(const char* varname)
{
    return getenv(varname);
}

bool Eco_LibC_FileExists(const char* path)
{
    struct stat sb;

    if (stat(path, &sb) < 0)
        return false;
    return true;
}

bool Eco_LibC_FileIsDirectory(const char* path)
{
    struct stat sb;

    if (stat(path, &sb) < 0)
        return false;
    return (sb.st_mode & S_IFMT) == S_IFDIR;
}

bool Eco_LibC_ListFiles(const char* path, char** entries, unsigned int max)
{
    DIR*            dir;
    struct dirent*  entry;
    unsigned int    index;
    unsigned int    count;

    count   = 0;
    dir     = opendir(path);

    if (dir != NULL) {
        while (true)
        {
            if (count >= (max - 1))
                break;
            entry = readdir(dir);
            if (entry == NULL)
                break;
            entries[count++] = Eco_LibC_Strdup(entry->d_name);
        }
        closedir(dir);

        for (index = 0; index < count; index++)
            entries[index] = entries[index];
        entries[index] = NULL;

        return true;
    } else {
        return false;
    }
}
