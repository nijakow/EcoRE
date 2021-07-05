#include <stdio.h>

#include "file.h"

#include "../instance.h"

#include "../../../vm/memory/memory.h"


bool Eco_EConnect_ReadFromFile(const char* filename, struct Eco_EConnect_Result* result)
{
    FILE*                         file;
    unsigned int                  file_size;
    char*                         buffer;
    struct Eco_EConnect_Instance  instance;
    struct Eco_EConnect_Reader    reader;
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
    fread(buffer, file_size, sizeof(char), file);

    fclose(file);

    Eco_EConnect_Instance_Create(&instance);
    Eco_EConnect_Reader_Create(&reader, &instance, buffer, file_size, (void (*)(char*)) Eco_Memory_Free);
    retval = Eco_EConnect_Reader_Read(&reader, result);
    Eco_EConnect_Reader_Destroy(&reader);
    Eco_EConnect_Instance_Destroy(&instance);

    return retval;
}
