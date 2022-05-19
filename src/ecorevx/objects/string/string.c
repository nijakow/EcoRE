#include "../../util/libc.h"

#include "string.h"

struct Eco_Object* Eco_String_New(Eco_Allocator_t allocator, const char* text)
{
    Eco_Size_t          length;
    Eco_Size_t          char_count;
    struct Eco_Object*  self;

    /*
     * Calculate the UTF-8 string length (in characters)
     */
    char_count = 0;
    for (length = 0; text[length] != '\0'; length++)
    {
        if ((text[length] & 0xc0) != 0x80)
            char_count++;
    }

    self = Eco_Object_AllocByteObject(allocator, NULL /* TODO! */, (length + 1));

    if (self != NULL)
    {
        Eco_String_SetCharacterCount(self, char_count);
        Eco_Util_Strcpy(Eco_ByteObject_GetBytePayload(self), text);
    }

    return self;
}
