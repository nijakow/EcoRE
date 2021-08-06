
#define DEFDISPATCH(N)  [Eco_Bytecode_ ## N] = &&dispatch_label_ ## N

static void* DISPATCH_TABLE[] = {
    DEFDISPATCH(NOOP),
    DEFDISPATCH(CONST),
    DEFDISPATCH(PUSHC),
    DEFDISPATCH(PUSH),
    DEFDISPATCH(POP),
    DEFDISPATCH(DROP),
    DEFDISPATCH(DUP),
    DEFDISPATCH(R2R),
    DEFDISPATCH(R2L),
    DEFDISPATCH(L2R),
    DEFDISPATCH(BUILTIN),
    DEFDISPATCH(SEND),
    DEFDISPATCH(ASSIGN),
    DEFDISPATCH(RETURN),
    DEFDISPATCH(MAKE_CLOSURE),
    DEFDISPATCH(SENDV)
};


#define NEXT_U8()             *(instruction++)
inline u16 CONSTRUCT_U16(u8** ptr) { u16 v = *((u16*) *ptr); *ptr += 2; return v; }
#define NEXT_U16()            CONSTRUCT_U16(&instruction)
#define NEXT_CONSTANT()       &top->code->constants[NEXT_U16()]
#define TARGET(T)             dispatch_label_ ## T:
#define DEFAULT_TARGET()      /* TARGET(DEFAULT) */
#define DISPATCH(B)           goto *DISPATCH_TABLE[B];
#define ULTRAFAST_DISPATCH()  DISPATCH(NEXT_U8())
#define FAST_DISPATCH()       goto short_retry
#define SLOW_DISPATCH()       goto long_retry

#define FAST_PUSH(V)               { Eco_Any_AssignAny((Eco_Any*) sp, V); sp += sizeof(Eco_Any); }
#define FAST_POP(V)                { sp -= sizeof(Eco_Any); Eco_Any_AssignAny(V, (Eco_Any*) sp); }
#define FAST_DROP()                { sp -= sizeof(Eco_Any); }
#define FAST_NTH(N)                ((Eco_Any*) (sp - (N) * sizeof(Eco_Any)))
#define FAST_DUP()                 { FAST_PUSH(FAST_NTH(1)); }

