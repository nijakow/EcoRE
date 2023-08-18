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
    char*              bytes;
    char               payload[];
};

struct Eco_String* Eco_String_New(const char*);
void Eco_String_MarkChildren(struct Eco_GC_State*, struct Eco_String*);
void Eco_String_MarkInstance(struct Eco_GC_State*, struct Eco_String*);
void Eco_String_Del(struct Eco_String*);
bool Eco_String_IsString(struct Eco_Object*);

static inline unsigned int Eco_String_ByteCount(struct Eco_String* string)
{
    return string->byte_count;
}

static inline unsigned int Eco_String_CharacterCount(struct Eco_String* string)
{
    return string->character_count;
}

static inline char Eco_String_ByteAt(struct Eco_String* string, unsigned int index)
{
    // TODO, FIXME, XXX: Watch out for out-of-bounds and overflow errors!
    return string->bytes[index];
}

static inline Eco_Codepoint Eco_String_At(struct Eco_String* string, unsigned int index)
{
    Eco_Codepoint  codepoint;
    unsigned int   length;

    if (Eco_Utf8_Decode(&string->bytes[index], &codepoint, &length)) {
        return codepoint;
    } else {
        return Eco_Utf8_REPLACEMENT_CHAR;
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


bool Eco_String_PutIntoByteArray(struct Eco_String*, char*, unsigned int);


struct Eco_Key;
struct Eco_Key* Eco_String_AsKey(struct Eco_String* string);

void Eco_String_Init();
void Eco_String_Terminate();

#endif
