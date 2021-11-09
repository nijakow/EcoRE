#ifndef ECO_OBJECTS_MISC_STRING_STRING_H
#define ECO_OBJECTS_MISC_STRING_STRING_H

#include <ecore/eco.h>

#include <ecore/objects/base/object.h>
#include <ecore/util/utf8.h>


struct Eco_String
{
    struct Eco_Object  _;
    unsigned int       byte_count;
    unsigned int       character_count;
    char               bytes[];
};

struct Eco_String* Eco_String_New(const char*);
void Eco_String_Mark(struct Eco_GC_State*, struct Eco_String*);
void Eco_String_Del(struct Eco_String*);

static inline unsigned int Eco_String_ByteCount(struct Eco_String* string)
{
    return string->byte_count;
}

static inline unsigned int Eco_String_CharacterCount(struct Eco_String* string)
{
    return string->character_count;
}

static inline Eco_Codepoint Eco_String_At(struct Eco_String* string, unsigned int index)
{
    Eco_Codepoint  codepoint;
    unsigned int   length;

    if (Eco_Utf8_Decode(&string->bytes[index], &codepoint, &length)) {
        return codepoint;
    } else {
        return -1;  // TODO: Make a special constant for this
    }
}

static inline unsigned int Eco_String_StepAt(struct Eco_String* string, unsigned int index)
{
    Eco_Codepoint  codepoint;
    unsigned int   length;

    if (Eco_Utf8_Decode(&string->bytes[index], &codepoint, &length)) {
        return length;
    } else {
        return 1;
    }
}

void Eco_String_Init();
void Eco_String_Terminate();

#endif
