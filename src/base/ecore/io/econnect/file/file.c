#include <stdio.h>

#include "file.h"

#include <ecore/io/econnect/instance.h>

#include <ecore/vm/memory/memory.h>


bool Eco_EConnect_LoadImageWithFreeFunc(struct Eco_EConnect_Result* result, char* image, unsigned long image_size, void* free_func)
{
    struct Eco_EConnect_Instance  instance;
    struct Eco_EConnect_Reader    reader;
    bool                          retval;

    Eco_EConnect_Instance_Create(&instance);
    Eco_EConnect_Reader_Create(&reader, &instance, image, image_size, free_func);
    retval = Eco_EConnect_Reader_Read(&reader, result);
    Eco_EConnect_Reader_Destroy(&reader);
    Eco_EConnect_Instance_Destroy(&instance);
    return retval;
}

bool Eco_EConnect_LoadImage(struct Eco_EConnect_Result* result, char* image, unsigned long image_size)
{
    return Eco_EConnect_LoadImageWithFreeFunc(result, image, image_size, NULL);
}

bool Eco_EConnect_ReadFile(const char* filename, struct Eco_EConnect_Result* result)
{
    FILE*                         file;
    unsigned long                 file_size;
    char*                         buffer;
    bool                          retval;

    file = fopen(filename, "rb");

    if (file == NULL) {
        Eco_EConnect_Result_Create_Error(result, Eco_EConnect_Result_ErrorType_RESOURCE_NOT_FOUND);
        return false;
    }

    fseek(file, 0L, SEEK_END);
    file_size = ftell(file);
    rewind(file);

    buffer = Eco_Memory_Alloc(file_size * sizeof(char));
    if (fread(buffer, file_size, sizeof(char), file) != 0) {
        retval = Eco_EConnect_LoadImageWithFreeFunc(result, buffer, file_size, Eco_Memory_Free);
    } else {
        retval = false;
    }

    fclose(file);

    return retval;
}
