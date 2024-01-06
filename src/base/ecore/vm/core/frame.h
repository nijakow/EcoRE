#ifndef ECO_VM_CORE_FRAME_H
#define ECO_VM_CORE_FRAME_H

#include <ecore/eco.h>
#include <ecore/objects/vm/code/code.h>


struct Eco_Environment;
struct Eco_GC_State;


struct Eco_Frame
{
    struct Eco_Frame*    previous;
    struct Eco_Frame*    return_to;
    struct Eco_Frame*    lexical;
    struct Eco_Closure*  closures;
    struct Eco_Closure*  handler;

    struct Eco_Key*      name;

    struct Eco_Code*     code;
    u8*                  instruction;

    Eco_Any              myself;
    
    unsigned int         named_arg_count;
    Eco_Any*             args;
    Eco_Any*             registers;
};


void Eco_Frame_Mark(struct Eco_GC_State*, struct Eco_Frame*);


static inline unsigned int Eco_Frame_ArgCount(struct Eco_Frame* frame)
{
    return frame->registers - frame->args;
}

static inline unsigned int Eco_Frame_VarargCount(struct Eco_Frame* frame)
{
    return Eco_Frame_ArgCount(frame) - frame->named_arg_count;
}


static inline u8 Eco_Frame_NextU8(struct Eco_Frame* frame)
{
    return *(frame->instruction++);
}

static inline u16 Eco_Frame_NextU16(struct Eco_Frame* frame)
{
    u16 v;

    v = Eco_Frame_NextU8(frame);
    v <<= 8;
    v |= Eco_Frame_NextU8(frame);

    return v;
}

static inline i32 Eco_Frame_NextI32(struct Eco_Frame* frame)
{
    u8  i;
    i32 v;

    v = 0;
    for (i = 0; i < 4; i++) {
        v <<= 8;
        v |= Eco_Frame_NextU8(frame);
    }

    return v;
}

static inline Eco_Any* Eco_Frame_NextConstant(struct Eco_Frame* frame)
{
    return &(frame->code->constants[Eco_Frame_NextU16(frame)]);
}

static inline struct Eco_Frame* Eco_Frame_NthLexical(struct Eco_Frame* frame, unsigned int depth)
{
    while (depth --> 0)
    {
        if (frame == NULL)
            return frame;
        frame = frame->lexical;
    }
    return frame;
}

#endif
