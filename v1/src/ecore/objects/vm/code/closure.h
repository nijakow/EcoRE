#ifndef ECO_OBJECTS_VM_CODE_CLOSURE_H
#define ECO_OBJECTS_VM_CODE_CLOSURE_H

#include <ecore/objects/base/object.h>


struct Eco_Code;
struct Eco_Frame;
struct Eco_GC_State;


struct Eco_Closure
{
    struct Eco_Object     _;

    struct Eco_Code*      code;
    struct Eco_Frame*     lexical;

    struct Eco_Closure**  prev;
    struct Eco_Closure*   next;
};


struct Eco_Closure* Eco_Closure_New(struct Eco_Code*, struct Eco_Frame*);
void Eco_Closure_Mark(struct Eco_GC_State*, struct Eco_Closure*);
void Eco_Closure_Del(struct Eco_Closure*);

void Eco_Closure_Init();
void Eco_Closure_Terminate();

#endif
