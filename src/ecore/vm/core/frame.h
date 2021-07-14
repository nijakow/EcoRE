#pragma once

#include "../../eco.h"
#include "../../objects/vm/code/code.h"


struct Eco_Environment;
struct Eco_GC_State;


struct Eco_Frame
{
    unsigned int             delta;

    Eco_Any                  self;

    struct Eco_Code*         code;
    u8*                      instruction;

    struct Eco_Environment*  dynamic_vars;

    unsigned int             register_count;
    Eco_Any                  registers[];
};


void Eco_Frame_Mark(struct Eco_GC_State*, struct Eco_Frame*);


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
