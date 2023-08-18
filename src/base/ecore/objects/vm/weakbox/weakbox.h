#ifndef ECO_OBJECTS_VM_WEAKBOX_WEAKBOX_H
#define ECO_OBJECTS_VM_WEAKBOX_WEAKBOX_H

#include <ecore/base/any.h>
#include <ecore/objects/base/object.h>


struct Eco_WeakBox
{
    struct Eco_Object     _;
    struct Eco_WeakBox**  prev;
    struct Eco_WeakBox*   next;
    Eco_Any               value;
};

struct Eco_WeakBox* Eco_WeakBox_New(unsigned int, unsigned int);
void Eco_WeakBox_MarkChildren(struct Eco_GC_State*, struct Eco_WeakBox*);
void Eco_WeakBox_MarkInstance(struct Eco_GC_State*, struct Eco_WeakBox*);
void Eco_WeakBox_Del(struct Eco_WeakBox*);

bool Eco_WeakBox_IsWeakBox(struct Eco_Object*);

Eco_Any Eco_WeakBox_GetValue(struct Eco_WeakBox*);
void Eco_WeakBox_SetValue(struct Eco_WeakBox*, Eco_Any);

void Eco_WeakBox_Init();
void Eco_WeakBox_Terminate();

#endif
