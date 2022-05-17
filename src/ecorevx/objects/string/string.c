#include "../../util/libc.h"

#include "string.h"

struct Eco_Object* Eco_String_New(Eco_Allocator_t allocator, const char* text)
{
    Eco_Size_t          length;
    struct Eco_Object*  self;

    length  = Eco_Util_Strlen(text);
    self    = Eco_Object_AllocByteObject(allocator, NULL /* TODO! */, (length + 1));

    if (self != NULL)
    {
        Eco_Util_Strcpy(Eco_ByteObject_GetBytePayload(self), text);
    }

    return self;
}
