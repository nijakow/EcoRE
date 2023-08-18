#include "weakbox.h"

#include <ecore/objects/base/object.h>
#include <ecore/objects/base/type.h>

#include <ecore/vm/memory/memory.h>
#include <ecore/vm/memory/gc/gc.h>


/*
 *    T y p e
 */

static struct Eco_TypeCore  Eco_WeakBox_TYPECORE;
       struct Eco_Type*     Eco_WeakBox_TYPE;


void Eco_WeakBox_Init()
{
    Eco_TypeCore_Create(&Eco_WeakBox_TYPECORE, "Eco_WeakBox");

    Eco_WeakBox_TYPECORE.send          = (Eco_TypeCore_SendFunc) Eco_Object_Send;
    Eco_WeakBox_TYPECORE.mark_instance = (Eco_TypeCore_MarkFunc) Eco_WeakBox_MarkInstance;
    Eco_WeakBox_TYPECORE.mark_children = (Eco_TypeCore_MarkFunc) Eco_WeakBox_MarkChildren;
    Eco_WeakBox_TYPECORE.del           = (Eco_TypeCore_DelFunc)  Eco_WeakBox_Del;

    Eco_WeakBox_TYPE                   = Eco_Type_NewPrefab(&Eco_WeakBox_TYPECORE);
}

void Eco_WeakBox_Terminate()
{
    Eco_TypeCore_Destroy(&Eco_WeakBox_TYPECORE);
}


/*
 *    B a s i c s
 */

static struct Eco_WeakBox* Eco_WEAKBOXES = NULL;

struct Eco_WeakBox* Eco_WeakBox_New()
{
    struct Eco_WeakBox*  weakbox;

    weakbox = Eco_Object_New(Eco_WeakBox_TYPE, sizeof(struct Eco_WeakBox));

    if (weakbox != NULL)
    {
        /*
         * By default, the weakbox references itself
         */
        weakbox->value = Eco_Any_FromPointer(weakbox);

        /*
         * Link the weakbox into the global list of weakboxes
         */
        {
            weakbox->prev = &Eco_WEAKBOXES;
            weakbox->next =  Eco_WEAKBOXES;
            if (Eco_WEAKBOXES != NULL)
                Eco_WEAKBOXES->prev = &weakbox->next;
            Eco_WEAKBOXES = weakbox;
        }
    }

    return weakbox;
}

void Eco_WeakBox_MarkChildren(struct Eco_GC_State* state, struct Eco_WeakBox* weakbox)
{
    /*
     * NO NEED TO MARK THE VALUE - IT IS A WEAK REFERENCE!
     */
    Eco_Object_MarkChildren(state, &(weakbox->_));
}

void Eco_WeakBox_MarkInstance(struct Eco_GC_State* state, struct Eco_WeakBox* weakbox)
{
    Eco_Object_MarkInstance(state, &(weakbox->_));
}

void Eco_WeakBox_Del(struct Eco_WeakBox* weakbox)
{
    if (weakbox->next != NULL)
        weakbox->next->prev = weakbox->prev;
    *weakbox->prev = weakbox->next;
    Eco_Object_Del(&(weakbox->_));
}

bool Eco_WeakBox_IsWeakBox(struct Eco_Object* object)
{
    return object->type->typecore == &Eco_WeakBox_TYPECORE;
}


Eco_Any Eco_WeakBox_GetValue(struct Eco_WeakBox* weakbox)
{
    return weakbox->value;
}

void Eco_WeakBox_SetValue(struct Eco_WeakBox* weakbox, Eco_Any new_value)
{
    struct Eco_Object*  object;

    if (Eco_Any_IsPointer(new_value))
    {
        object = Eco_Any_AsPointer(new_value);
        object->bits.weakly_referenced = true;
    }

    weakbox->value = new_value;
}


void Eco_WeakBox_NotifyRelease(Eco_Any value)
{
    struct Eco_WeakBox*  weakbox;

    for (weakbox = Eco_WEAKBOXES; weakbox != NULL; weakbox = weakbox->next)
    {
        if (Eco_Any_Equals(weakbox->value, value))
        {
            /*
             * Make the weakbox point to itself again
             */
            weakbox->value = Eco_Any_FromPointer(weakbox);
        }
    }
}
