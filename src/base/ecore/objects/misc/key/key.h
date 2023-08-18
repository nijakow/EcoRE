#ifndef ECO_OBJECTS_MISC_KEY_KEY_H
#define ECO_OBJECTS_MISC_KEY_KEY_H

#include <ecore/eco.h>

#include <ecore/objects/base/object.h>

#include <ecore/io/econnect/reader/callback.h>
#include <ecore/vm/builtins/builtin.h>


struct Eco_Key
{
    struct Eco_Object  _;

    struct Eco_Key*        next;
    struct Eco_Key*        prev;

    Eco_EConnect_Callback  econnect_callback;
    Eco_Builtin            builtin;

    char                   name[];
};

struct Eco_Key* Eco_Key_Find(const char*);
struct Eco_Key* Eco_Key_Gensym();

void Eco_Key_MarkChildren(struct Eco_GC_State*, struct Eco_Key*);
void Eco_Key_MarkInstance(struct Eco_GC_State*, struct Eco_Key*);
void Eco_Key_Del(struct Eco_Key*);

static inline bool Eco_Key_CallBuiltin(struct Eco_Key* key, struct Eco_Fiber* fiber, unsigned int args)
{
    return key->builtin(fiber, args);
}

struct Eco_String;
struct Eco_String* Eco_Key_AsString(struct Eco_Key*);

void Eco_Key_Init();
void Eco_Key_Terminate();

#endif
